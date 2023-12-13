import tensorflow as tf
from tensorflow import keras
import matplotlib.pyplot as plt
import tkinter as tk
from PIL import Image, ImageOps
import numpy as np
from tensorflow.keras.models import load_model
import os

model = tf.keras.models.load_model('modelProjet')

def dessiner(event):
    x, y = event.x, event.y
    canevas.create_oval(x - 10, y - 10, x + 10, y + 10, fill="black")


def effacer_canevas():
    canevas.delete("all")

def effacer_droit(events):
    canevas.delete("all")


fenetre = tk.Tk()
fenetre.title("Dessin en Python")


canevas = tk.Canvas(fenetre, width=400, height=400, bg="white")
canevas.pack()


effacer_bouton = tk.Button(fenetre, text="Effacer", command=effacer_canevas)
effacer_bouton.pack()



canevas.bind("<B1-Motion>", dessiner)
canevas.bind("<Button-3>",effacer_droit)

def sauvegarder():
    canevas.postscript(file="temp.ps", colormode="color")
    image_ps = Image.open("temp.ps")
    image_ps.save('temp.png','PNG')

def guess():
    sauvegarder()

    image = Image.open('temp.png')
    image = image.resize((28,28))
    image = image.convert('L')
    image = ImageOps.invert(image)
    image_array = np.array(image)
    image_array = image_array / 255
    image_array = image_array.reshape(1,28,28,1)


    prediction = model.predict(image_array)
    print("prediction : ", prediction)

    predicted_num = np.argmax(prediction)
    if predicted_num == 0:
        mot = "avion"
    elif predicted_num == 1:
        mot = "pomme"
    elif predicted_num == 2:
        mot = "hache"
    elif predicted_num == 3:
        mot = "basket-ball"
    elif predicted_num == 4:
        mot = "pain"
    elif predicted_num == 5:
        mot = "tour eiffel"
    else :
        mot = "pafweeeeee"

    print("Je sais ! , c'est ", mot, " !")
    os.remove("temp.ps")
    os.remove("temp.png")




sauvegarder_bouton = tk.Button(fenetre, text="Predire le dessin", command=guess)
sauvegarder_bouton.pack()


fenetre.mainloop()