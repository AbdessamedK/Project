import tkinter as tk
from PIL import Image, ImageOps
import numpy as np
from tensorflow.keras.models import load_model

modele = load_model('modelechiffre.h5')

# Fonction pour dessiner sur le canevas
def dessiner(event):
    x, y = event.x, event.y
    canevas.create_oval(x - 10, y - 10, x + 10, y + 10, fill="black")

# Fonction pour effacer le canevas
def effacer_canevas():
    canevas.delete("all")
    
def effacer_droit(events):
    canevas.delete("all")

# Créer une fenêtre
fenetre = tk.Tk()
fenetre.title("Dessin en Python")

# Créer un canevas pour dessiner
canevas = tk.Canvas(fenetre, width=400, height=400, bg="white")
canevas.pack()

# Créer un bouton pour effacer le canevas
effacer_bouton = tk.Button(fenetre, text="Effacer", command=effacer_canevas)
effacer_bouton.pack()

# Lier la fonction de dessin à l'événement de clic de la souris
# Comme en prog fonctionnelle lui passer une fonction pour faire une fonction
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
    image_array = np.reshape(image,(1,784))
    image_array = image_array/255

    prediction = modele.predict(image_array)
    print("prediction : ", prediction)
    predicted_num = np.argmax(prediction)
    print("Num predit:",predicted_num)
    
    
# Créer un bouton pour sauvegarder le dessin comme une image
sauvegarder_bouton = tk.Button(fenetre, text="Predire le chiffre", command=guess)
sauvegarder_bouton.pack()

# Lancer la boucle principale
fenetre.mainloop()

