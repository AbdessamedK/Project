import socket
import cv2
import numpy as np
import struct
import threading
import select
import cv2.aruco as aruco  # Pour détection ArUco

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

def detect_balls(img):
    output = img.copy()
    hsv = cv2.cvtColor(output, cv2.COLOR_BGR2HSV)

    # --- Détection de la balle rouge ---
    lower_red1 = np.array([0, 100, 100])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([160, 100, 100])
    upper_red2 = np.array([180, 255, 255])
    mask_red1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask_red2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask_red = cv2.add(mask_red1, mask_red2)

    # --- Détection de la balle bleue ---
    lower_blue = np.array([100, 150, 0])
    upper_blue = np.array([140, 255, 255])
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

    # Paramètres de filtrage
    min_area = 5
    max_area = 90
    min_circularity = 0.6

    # Listes pour stocker les positions
    yellow_positions = []
    pink_positions = []

    def detect_and_draw(mask, point_color, image, positions_list):
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            if min_area < area < max_area:
                perimeter = cv2.arcLength(cnt, True)
                if perimeter > 0:
                    circularity = 4 * np.pi * area / (perimeter * perimeter)
                    if circularity >= min_circularity:
                        M = cv2.moments(cnt)
                        if M["m00"] != 0:
                            cx = int(M["m10"] / M["m00"])
                            cy = int(M["m01"] / M["m00"])
                            cv2.circle(image, (cx, cy), 5, point_color, -1)
                            positions_list.append((cx, cy))

    detect_and_draw(mask_red, (255, 0, 255), output, pink_positions)   # Rose
    detect_and_draw(mask_blue, (0, 255, 255), output, yellow_positions) # Jaune

    return output, yellow_positions, pink_positions

def direction_to_closest(x, y, positions):
    if not positions:
        return None  # Aucun point cible

    current = np.array([x, y])
    # Calcul de la distance à chaque position
    distances = [np.linalg.norm(np.array(p) - current) for p in positions]
    
    # Index de la position la plus proche
    closest_idx = np.argmin(distances)
    closest_pos = np.array(positions[closest_idx])
    
    # Vecteur direction
    direction = closest_pos - current
    norm = np.linalg.norm(direction)

    if norm == 0:
        return (0.0, 0.0)  # Déjà sur la cible

    # Vecteur unitaire (direction normalisée)
    direction_normalized = direction / norm
    return tuple(direction_normalized)

def detect_aruco(image):
    output = image.copy()
    gray = cv2.cvtColor(output, cv2.COLOR_BGR2GRAY)
    aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_6X6_250)
    parameters = aruco.DetectorParameters()
    detector = aruco.ArucoDetector(aruco_dict, parameters)

    corners, ids, _ = detector.detectMarkers(gray)
    positions = {}

    if ids is not None:
        aruco.drawDetectedMarkers(output, corners, ids)
        for i, corner in enumerate(corners):
            c = corner[0]
            center_x = int(np.mean(c[:, 0]))
            center_y = int(np.mean(c[:, 1]))
            aruco_id = int(ids[i][0])
            positions[aruco_id] = (center_x, center_y)
            
            # Dessin du centre et de l'ID
            cv2.circle(output, (center_x, center_y), 5, (255, 0, 0), -1)
            cv2.putText(output, f"ID: {aruco_id}", (center_x + 5, center_y - 5),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 0, 0), 2)

    return output, positions


# Boucle principale du programme
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
    
    roi_mask = cv2.imread("masque.png", cv2.IMREAD_GRAYSCALE)
    if roi_mask is None:
        print("Erreur: le masque n'a pas pu être chargé.")
        return
    roi_mask = roi_mask.astype(np.uint8)
    
    stopProgram.clear()
    stopThread.clear()
    thread = threading.Thread(target=captureThread, args=[sock, frame, stopThread, threadRunning], daemon=True)
    thread.start()
    
    while not stopProgram.is_set():
        if frame is not None:
            roi_frame = cv2.bitwise_and(frame.mat, frame.mat, mask=roi_mask)
            processed, yellow, pink = detect_balls(roi_frame)
            processed, positions = detect_aruco(processed)
            if positions.get(8) is not None:
                x = positions.get(7)[0]
                y = positions.get(7)[1]
                direction = direction_to_closest(x,y,pink)
                if direction is not None:
                    dx, dy = direction
                    print(positions)
                    scale = 50
                    end_point = (int(x + dx * scale), int(y + dy * scale))
                    cv2.arrowedLine(processed, (x, y), end_point, (0, 255, 0), 2, tipLength=0.3)
            cv2.imshow("Détection balles + QR code", processed)
        ch = chr(cv2.waitKey(1) & 0xFF)
        if ch in ['q', 'Q']:
            stopProgram.set()
    
    stopThread.set()
    while threadRunning.is_set():
        pass
    sock.close()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()
