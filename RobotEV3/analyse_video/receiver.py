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

def main():
    stopProgram = threading.Event()
    stopThread = threading.Event()
    threadRunning = threading.Event()
    # Configuration
    ip = ""  # listen to all interfaces
    ip = ""  # listen to specific interface
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
            cv2.imshow("Received Frame", frame.mat)
            cv2.imwrite("imageC.png", frame.mat)
        ch = chr(cv2.waitKey(int(1)) & 0xFF)
        if ch=='q' or ch=='Q': stopProgram.set()
    stopThread.set()
    while threadRunning.is_set(): pass
    sock.close()
    cv2.destroyAllWindows()

main()
