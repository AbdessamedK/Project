#!/usr/bin/env pybricks-micropython
from pybricks.hubs import EV3Brick
from pybricks.ev3devices import Motor, GyroSensor
from pybricks.parameters import Port
from pybricks.tools import wait
import socket
import select

matrice = [
    [-0.33, 0.58, 0.33],
    [-0.33, -0.58, 0.33],
    [0.67, 0, 0.33]
]

ev3 = EV3Brick()
ev3.speaker.beep()

motor_1 = Motor(Port.A)
motor_2 = Motor(Port.B)
motor_3 = Motor(Port.C)

gyro_sensor = GyroSensor(Port.S1)

HOST = "0.0.0.0"  
PORT = 12345     

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)

print("En attente de connexion Wi-Fi...")
client, addr = server.accept()
print("Connecté à {}".format(addr))
print("Type de client:", type(client))

running = True
seuilAngle = 5

while running:
    try:
        angle = gyro_sensor.angle()

        message = "Gyro Angle: {}".format(angle)
        client.send(message.encode())

        data = client.recv(1024).decode().strip()
        if not data:
            continue
        if data == "STOP":
            running = False
            break

        f1, f2, f3 = map(float, data.split(","))

        if angle<-seuilAngle or angle>seuilAngle:
            print("je me reajuste")
            print("-angle : ", -angle)
            print("angle : ", angle)
            vecteur = [0,0,-angle]
            if angle < 0:
                vecteur = [0,0,min(-angle, 10)]
            else:
                vecteur = [0,0,max(-angle, -10)]
            f1,f2,f3 = [sum(matrice[i][j] * vecteur[j] for j in range(len(vecteur))) for i in range(len(matrice))]
            f1 = f1 * 5
            f2 = f2 * 5
            f3 = f3 * 5
        
        vecteur = [0,0,10]
        f1,f2,f3 = [sum(matrice[i][j] * vecteur[j] for j in range(len(vecteur))) for i in range(len(matrice))]
        f1 = f1 * 5
        f2 = f2 * 5
        f3 = f3 * 5

        motor_1.dc(f1)
        motor_2.dc(f2)
        motor_3.dc(f3)
    
    except Exception as e:
        print("Erreur : {}".format(e))
        break

motor_1.stop()
motor_2.stop()
motor_3.stop()
client.close()
server.close()
print("Connexion fermée.")
