import math
import time
import socket 
import cv2
import numpy as np
import select

matrice = [
    [-0.33, 0.58, 0.33],
    [-0.33, -0.58, 0.33],
    [0.67, 0, 0.33]
]

class VirtualJoystick:
    def __init__(self, size=300):
        self.size = size
        self.center = (size // 2, size // 2)
        self.pos = self.center
        self.active = False
        self.window_name = "Joystick"

        cv2.namedWindow(self.window_name)
        cv2.setMouseCallback(self.window_name, self.mouse_event)

    def mouse_event(self, event, x, y, flags, params):
        if event == cv2.EVENT_LBUTTONDOWN:
            self.active = True
            self.pos = (x, y)
        elif event == cv2.EVENT_MOUSEMOVE and self.active:
            self.pos = (x, y)
        elif event == cv2.EVENT_LBUTTONUP:
            self.active = False
            self.pos = self.center

    def get_vector(self):
        dx = self.pos[0] - self.center[0]
        dy = self.center[1] - self.pos[1]

        norm = np.hypot(dx, dy)
        radius = self.size // 2
        if norm > radius:
            dx = dx * radius / norm
            dy = dy * radius / norm
            norm = radius

        return (dx / radius, dy / radius)

    def draw(self):
        canvas = np.zeros((self.size, self.size, 3), dtype=np.uint8)
        cv2.circle(canvas, self.center, self.size // 2, (100, 100, 100), 2)
        cv2.circle(canvas, self.pos, 10, (0, 255, 0), -1)
        cv2.imshow(self.window_name, canvas)


EV3_IP = "172.20.10.3"
PORT = 12345

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#client.connect((EV3_IP, PORT))

print("Connecté au robot EV3 via Wi-Fi")

def main():
    running = True
    joystick = VirtualJoystick()
    horloge = 0
    
    while running:
        joystick.draw()
        
        #x,y = map(float, input("Donnez la direction \n").split())

        x, y = joystick.get_vector()
        
        vecteur = [-x, y, 0]

        f1,f2,f3 = [sum(matrice[i][j] * vecteur[j] for j in range(len(vecteur))) for i in range(len(matrice))]
        speed_motor1 = f1 * 200
        speed_motor2 = f2 * 200
        speed_motor3 = f3 * 200
        message = f"{speed_motor1},{speed_motor2},{speed_motor3}"

        if(time.time() - horloge >= 0.25):
            #client.sendall(message.encode())
            horloge = time.time()


        ch = chr(cv2.waitKey(1) & 0xFF)
        if ch in ['q', 'Q']:
            running = False

    cv2.destroyAllWindows()
    client.close()
    print("Connexion fermée.")

if __name__ == '__main__':
    main()