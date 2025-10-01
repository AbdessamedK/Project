import socket
import cv2
import numpy as np
import struct
import threading
import select

class Frame:
    def __init__(self,w,h):
        self.mat = np.zeros((h,w,3),dtype=np.uint8)
        self.id = -1

def incomingFrame(frame,iframe, frame_id):
    iframe.mat = frame.copy()
    iframe.id = frame_id

def gotFullData(data_buffer,iframe, frame_id):
    # Reconstruct the full frame
    full_data = b''.join([data_buffer[i] for i in sorted(data_buffer)])
    messageLength = struct.unpack("I",full_data[0:4])[0]
    frame_data = full_data[4:]
    if len(frame_data) == messageLength:
        #do we have all data
        frame_buffer = np.frombuffer(frame_data, dtype=np.uint8)
        frame = cv2.imdecode(frame_buffer,1)
        if frame is not None:
            # is frame ok
            incomingFrame(frame,iframe, frame_id)

def captureThread(sock,frame,stopThread,threadRunning):
    MaximumPacketSize = 1400
    timeout_ms = 0.01
    data_buffer = {}
    current_frame_id = -1
    threadRunning.set()
    while not stopThread.is_set():
        try:
            read_ready, _, _ = select.select([sock], [], [], timeout_ms)
            readSet = bool(read_ready)  # True if data is ready to be read
            if read_ready and readSet:
                packet, addr = sock.recvfrom(MaximumPacketSize)
                packet_id, frame_id = struct.unpack('II', packet[:8])
                payload = packet[8:]
                if frame_id != current_frame_id:
                    if current_frame_id != -1:
                        gotFullData(data_buffer,frame, frame_id)
                    # Reset buffer for new frame
                    data_buffer = {}
                    current_frame_id = frame_id
                data_buffer[packet_id] = payload
        except socket.error:
            continue
    threadRunning.clear()

import cv2
import numpy as np

import cv2
import numpy as np

def detect_and_mark_balls(image):
    # Convertir l'image en HSV
    hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

    # --- Plages pour le rouge (2 plages), inchangées ---
    lower_red1 = np.array([0, 120, 70])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([170, 120, 70])
    upper_red2 = np.array([180, 255, 255])
    
    # Masque rouge combiné
    mask_red1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask_red2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask_red = mask_red1 + mask_red2

    # --- Plages pour le bleu (2 plages) ---
    # Ajustez si nécessaire selon votre balle bleue. 
    # Par exemple, si elle est très foncée, descendez encore S_min, V_min.
    lower_blue1 = np.array([90,  50,  50])  
    upper_blue1 = np.array([110, 255, 255])
    lower_blue2 = np.array([111, 50,  50])
    upper_blue2 = np.array([130, 255, 255])

    # Masques bleus séparés, puis combinés
    mask_blue1 = cv2.inRange(hsv, lower_blue1, upper_blue1)
    mask_blue2 = cv2.inRange(hsv, lower_blue2, upper_blue2)
    mask_blue  = mask_blue1 + mask_blue2

    # (Optionnel) Visualiser le masque bleu pour déboguer
    cv2.imshow("mask_blue", mask_blue)

    # --- Opérations morphologiques pour réduire le bruit ---
    kernel = np.ones((5, 5), np.uint8)
    mask_red  = cv2.morphologyEx(mask_red,  cv2.MORPH_OPEN,  kernel, iterations=2)
    mask_red  = cv2.morphologyEx(mask_red,  cv2.MORPH_CLOSE, kernel, iterations=2)
    mask_blue = cv2.morphologyEx(mask_blue, cv2.MORPH_OPEN,  kernel, iterations=2)
    mask_blue = cv2.morphologyEx(mask_blue, cv2.MORPH_CLOSE, kernel, iterations=2)

    # --- Détecter les cercles ROUGES (pour dessiner un point BLEU) ---
    circles_red = cv2.HoughCircles(
        mask_red, 
        cv2.HOUGH_GRADIENT, 
        dp=1.2, 
        minDist=30,
        param1=50, 
        param2=25,   # Augmentez si trop de faux cercles, réduisez si pas assez
        minRadius=10, 
        maxRadius=100
    )
    if circles_red is not None:
        circles_red = np.uint16(np.around(circles_red))
        for circle in circles_red[0, :]:
            x, y, r = circle
            # On dessine un point BLEU (BGR = (255, 0, 0))
            cv2.circle(image, (x, y), 5, (255, 0, 0), -1)

    # --- Détecter les cercles BLEUS (pour dessiner un point ROUGE) ---
    circles_blue = cv2.HoughCircles(
        mask_blue, 
        cv2.HOUGH_GRADIENT, 
        dp=1.2, 
        minDist=30,
        param1=50, 
        param2=25,
        minRadius=10, 
        maxRadius=100
    )
    if circles_blue is not None:
        circles_blue = np.uint16(np.around(circles_blue))
        for circle in circles_blue[0, :]:
            x, y, r = circle
            # On dessine un point ROUGE (BGR = (0, 0, 255))
            cv2.circle(image, (x, y), 5, (0, 0, 255), -1)

    return image



def main():
    stopProgram = threading.Event()
    stopThread = threading.Event()
    threadRunning = threading.Event()
    # Configuration
    ip = ""  # listen to all interfaces
    #ip = "192.168.1.186"  # listen to specific interface
    port = 8080
    # Create UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setblocking(False)
    sock.bind((ip, port))
    print("Listening for UDP frames...")
    frame = Frame(100,100)
    stopProgram.clear()
    stopThread.clear()
    thread = threading.Thread(target=captureThread,args = [sock,frame,stopThread,threadRunning], daemon=True)
    thread.start()
    while not stopProgram.is_set():
        if frame is not None:
            frame.mat = detect_and_mark_balls(frame.mat)
            cv2.imshow("Received Frame", frame.mat)
        ch = chr(cv2.waitKey(int(1)) & 0xFF)
        if ch=='q' or ch=='Q': stopProgram.set()
    stopThread.set()
    while threadRunning.is_set(): pass
    sock.close()
    cv2.destroyAllWindows()

main()