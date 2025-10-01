import math
import time
import socket 
import cv2
import numpy as np
import struct
import threading
import select
import cv2.aruco as aruco


# ----------- Polygon setup -----------
n = 20  # nombre de côtés
L = 70  # longueur en cm
R = L / (2 * np.sin(np.pi / n))

angles = np.linspace(0, 2 * np.pi, n, endpoint=False)
points_polygon = np.stack([
    R * np.cos(angles),
    R * np.sin(angles)
], axis=-1).astype(np.float32)

points_closed = np.vstack([points_polygon, points_polygon[0]])

center_point = np.array([[0.0, 0.0]], dtype=np.float32)

# ----------- Homographie bas -----------
pts_img1 = np.array([
    [527, 44],
    [458, 24],
    [396, 17],
    [333, 18],
    [272, 28],
    [208, 47],
    [151, 77],
    [104, 121],
    [63, 168],
    [43, 232],
    [47, 303],
    [592, 76],
    [326, 156]
], dtype=np.float32)

indices1 = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 19]
pts_poly1 = np.vstack([points_polygon[indices1], center_point])

H1, _ = cv2.findHomography(pts_img1, pts_poly1)

# ----------- Homographie haut -----------
pts_img2 = np.array([
    [427, 34],
    [370, 32],
    [311, 40],
    [258, 54],
    [208, 78],
    [166, 111],
    [135, 152],
    [119, 200],
    [128, 258],
    [366, 142]
], dtype=np.float32)

indices2 = [11, 12, 13, 14, 15, 16, 17, 18, 19]
pts_poly2 = np.vstack([points_polygon[indices2], center_point])

H2, _ = cv2.findHomography(pts_img2, pts_poly2)

def image_to_plan(point_img, H):
    pt = np.array([[point_img]], dtype=np.float32)
    pt_transformed = cv2.perspectiveTransform(pt, H)
    return pt_transformed[0][0] 

def distance(p1, p2):
    return math.sqrt((p2[0] - p1[0])**2 + (p2[1] - p1[1])**2)
    
def moyenne(a,b):
    return (a+b)/2

def moyenne_tab(tab, seuil):
    count = 0
    ret = []
    for (i1,i2) in tab:
        found=False
        curr = (i1,i2)
        found2 = 0
        for (j1,j2) in tab:
            if ((i1,i2)==(j1,j2)):
                continue
            else:
                curr2 = (j1,j2)
                if distance(curr,curr2) <= seuil:
                    found=True
                    found2 = curr2
                    ret.append((moyenne(i1,j1),moyenne(i2,j2)))
                    break
            count+=1
        if found == True:
            tab.remove(curr)
            tab.remove(found2)
    return ret + tab

# Classe pour stocker la frame
class Frame:
    def __init__(self, w, h):
        self.mat = np.zeros((h, w, 3), dtype=np.uint8)
        self.id = -1

# Met à jour la frame reçue
def incomingFrame(frame, iframe, frame_id):
    iframe.mat = frame.copy()
    iframe.id = frame_id

# Reconstitue et décode la frame complète à partir des paquets UDP
def gotFullData(data_buffer, iframe, frame_id):
    full_data = b''.join([data_buffer[i] for i in sorted(data_buffer)])
    messageLength = struct.unpack("I", full_data[0:4])[0]
    frame_data = full_data[4:]
    if len(frame_data) == messageLength:
        frame_buffer = np.frombuffer(frame_data, dtype=np.uint8)
        frame = cv2.imdecode(frame_buffer, 1)
        if frame is not None:
            incomingFrame(frame, iframe, frame_id)

# Thread de capture UDP
def captureThread(sock, frame, stopThread, threadRunning):
    MaximumPacketSize = 1400
    timeout_ms = 0.01
    data_buffer = {}
    current_frame_id = -1
    threadRunning.set()
    while not stopThread.is_set():
        try:
            read_ready, _, _ = select.select([sock], [], [], timeout_ms)
            if read_ready:
                packet, addr = sock.recvfrom(MaximumPacketSize)
                packet_id, frame_id = struct.unpack('II', packet[:8])
                payload = packet[8:]
                if frame_id != current_frame_id:
                    if current_frame_id != -1:
                        gotFullData(data_buffer, frame, frame_id)
                    data_buffer = {}
                    current_frame_id = frame_id
                data_buffer[packet_id] = payload
        except socket.error:
            continue
    threadRunning.clear()

# Fonction de détection des balles dans une image
def detect_balls(img):
    output = img.copy()
    hsv = cv2.cvtColor(output, cv2.COLOR_BGR2HSV)

    lower_red1 = np.array([0, 100, 100])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([160, 100, 100])
    upper_red2 = np.array([180, 255, 255])
    mask_red = cv2.add(cv2.inRange(hsv, lower_red1, upper_red1),
                       cv2.inRange(hsv, lower_red2, upper_red2))

    lower_blue = np.array([100, 150, 0])
    upper_blue = np.array([140, 255, 255])
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

    lower_green = np.array([40, 60, 60]) 
    upper_green = np.array([90, 255, 255])
    mask_green_square = cv2.inRange(hsv, lower_green, upper_green)

    min_area = 5
    max_area = 90
    min_circularity = 0.6
    min_square_area = 100 

    yellow_ball_positions = []
    pink_positions = []
    yellow_square_positions = []
    pink_res = []
    yellow_res = []
    square_res = []

    def detect_and_draw(mask, point_color, image, positions_list, shape_filter=None, area_range=None):
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area_range and not (area_range[0] <= area <= area_range[1]):
                continue
            perimeter = cv2.arcLength(cnt, True)
            if perimeter == 0:
                continue
            if shape_filter == "circle":
                circularity = 4 * np.pi * area / (perimeter * perimeter)
                if circularity < min_circularity:
                    continue
            elif shape_filter == "square":
                approx = cv2.approxPolyDP(cnt, 0.04 * perimeter, True)
                if len(approx) != 4:
                    continue
            M = cv2.moments(cnt)
            if M["m00"] != 0:
                cx = int(M["m10"] / M["m00"])
                cy = int(M["m01"] / M["m00"])
                cv2.circle(image, (cx, cy), 5, point_color, -1)
                positions_list.append((cx, cy))

    detect_and_draw(mask_red, (255, 0, 255), output, pink_positions, shape_filter="circle", area_range=(min_area, max_area))
    detect_and_draw(mask_blue, (0, 255, 255), output, yellow_ball_positions, shape_filter="circle", area_range=(min_area, max_area))

    detect_and_draw(mask_green_square, (0, 255, 0), output, yellow_square_positions, shape_filter="square", area_range=(min_square_area, 10000))

    def convert_positions(positions, H1, H2):
        results = []
        for (x, y) in positions:
            y_adjusted = y - (img.shape[0] / 2) if y >= (img.shape[0] / 2) else y
            H = H1 if y >= (img.shape[0] / 2) else H2
            pt_polygon = image_to_plan((x, y_adjusted), H)
            results.append(pt_polygon)
        return [(round(float(p[0]), 2), round(float(p[1]), 2)) for p in results]

    pink_res = moyenne_tab(convert_positions(pink_positions, H1, H2), 20)
    yellow_res = moyenne_tab(convert_positions(yellow_ball_positions, H1, H2), 20)
    square_res = moyenne_tab(convert_positions(yellow_square_positions, H1, H2), 20)

    return output, yellow_res, pink_res, square_res


def direction_to_closest(x, y, positions):
    if not positions:
        return None 

    current = np.array([x, y])
    distances = [np.linalg.norm(np.array(p) - current) for p in positions]
    
    closest_idx = np.argmin(distances)
    closest_pos = np.array(positions[closest_idx])
    
    direction = closest_pos - current
    norm = np.linalg.norm(direction)

    if norm == 0:
        return (0.0, 0.0)

    direction_normalized = direction / norm
    return tuple(direction_normalized)


matrice = [
    [-0.33, 0.58, 0.33],
    [-0.33, -0.58, 0.33],
    [0.67, 0, 0.33]
]

EV3_IP = "172.20.10.3"
PORT = 12345

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((EV3_IP, PORT))

print("Connecté au robot EV3 via Wi-Fi")

def main():
    stopProgram = threading.Event()
    stopThread = threading.Event()
    threadRunning = threading.Event()
    
    ip = ""  # écouter sur toutes les interfaces
    port = 8080
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setblocking(False)
    sock.bind((ip, port))
    print("En écoute des frames UDP...")
    
    frame = Frame(640, 720)
    horloge = 0
    
    roi_mask = cv2.imread("imageC.png", cv2.IMREAD_GRAYSCALE)
    if roi_mask is None:
        print("Erreur: le masque n'a pas pu être chargé.")
        return
    roi_mask = roi_mask.astype(np.uint8)
    
    stopProgram.clear()
    stopThread.clear()
    thread = threading.Thread(target=captureThread, args=[sock, frame, stopThread, threadRunning], daemon=True)
    thread.start()
    
    while not stopProgram.is_set():
        '''data = client.recv(1024).decode().strip()
        if data:
            print(f"💬 Message du robot : {data}")'''

        if frame is not None:
            roi_frame = cv2.bitwise_and(frame.mat, frame.mat, mask=roi_mask)
            processed, yellow, pink, robot_pos = detect_balls(roi_frame)
            
            #processed, positions = detect_aruco(processed)

            if robot_pos:
                x,y = robot_pos[0]
                direction = direction_to_closest(x,y,pink)
                dx, dy = direction
                vecteur = [-dx, dy, 0]
                resultat = [sum(matrice[i][j] * vecteur[j] for j in range(len(vecteur))) for i in range(len(matrice))]
                f1,f2,f3 = resultat
                speed_motor1 = f1 * 150
                speed_motor2 = f2 * 150
                speed_motor3 = f3 * 150
                message = f"{speed_motor1},{speed_motor2},{speed_motor3}"
                if(time.time() - horloge >= 0.25):
                    client.sendall(message.encode())
                    horloge = time.time()
            

            cv2.imshow("Détection balles + QR code", processed)
        ch = chr(cv2.waitKey(1) & 0xFF)
        if ch in ['q', 'Q']:
            stopProgram.set()
    
    stopThread.set()
    while threadRunning.is_set():
        pass
    sock.close()
    cv2.destroyAllWindows()
    client.close()
    print("Connexion fermée.")

if __name__ == '__main__':
    main()