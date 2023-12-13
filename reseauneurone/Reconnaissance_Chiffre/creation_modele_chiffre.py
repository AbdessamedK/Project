import matplotlib.pyplot as plt
import numpy as np
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense


from tensorflow.keras.datasets import mnist
from tensorflow.keras.utils import to_categorical

(X_train_data, Y_train_data), (X_test_data, Y_test_data) = mnist.load_data()

N = X_train_data.shape[0] # shape -> pour la taille du tableau

X_train = np.reshape(X_train_data,(N,784)) # Pour 
# mettre en 2d le tableau d'image d'entrainement donc un tableau de 60000x784

X_train = X_train / 255 # Pour normaliser car entre 0 et 255 le niveau 
# de gris pour le passer de 0 à 1 

Y_train = to_categorical(Y_train_data,num_classes=10)
# tableau resultat pour le passer : [5,3,...] -> [{0,0,0,0,0,1,0,...},...]

plt.imshow(X_test_data[10])

X_test = np.reshape(X_test_data,(X_test_data.shape[0],784))
X_test = X_test/255
Y_test = to_categorical(Y_test_data,num_classes=10)

# Modele du réseaux de neuronne

p = 128
modele = Sequential()

# Premiere couche qui prend les 784 inputs
modele.add(Dense(p, input_dim=784, activation='relu'))

# Deuxieme couche avec p neuronnes
modele.add(Dense(p, activation='relu'))

# Couche de sortie 10 neuronnes (un par chiffre)
# softmax pour que la somme des resultat de tout les neuronnes
# fassent 1
modele.add(Dense(10, activation='softmax'))

# loss = ... fonction predefini pour le loss
# optimizer = sgd une fonction qui ajuste le poids des neuronnes
# metrics = pour analyser les performences, stats
modele.compile(loss='categorical_crossentropy',
              optimizer='adam',
              metrics=['accuracy'])

print(modele.summary())

# Methode pour optimiser les poids
# epochs = le nb de fois qu'il lit toute les donnes pour ajuster les
# poids 
# batch_size : Prendre les 60k image de 32 en 32
modele.fit(X_train, Y_train, batch_size=32, epochs=10)
modele.fit(X_test, Y_test, batch_size=32, epochs=2)

modele.save('modelechiffre.h5')

