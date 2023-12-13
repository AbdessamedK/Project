import numpy as np
import matplotlib.pyplot as plt
from keras.datasets import mnist
from keras.models import Model
from keras.layers import Input, Dense

(X_train, _), (X_test, _) = mnist.load_data()
N = X_train.shape[0] 
Xtrain = np.reshape(X_train,(N,784))
Xtest = np.reshape(X_test,(X_test.shape[0],784))


Xtrain = Xtrain.astype("float32")/255.
Xtest = Xtest.astype("float32")/255.

print('X_train shape:', X_train.shape)
print(X_train.shape[0], 'train samples')
print(X_test.shape[0], 'test samples')

input_size = 784
hidden_size = 64
output_size = 784
x = Input(shape=(input_size,)) 
# Rien apres la virgule juste pour cree un tuple et pour
# que sa soit de la bonne forme
h = Dense(hidden_size, activation='relu')(x)
# Couche Dense connecte a x
r = Dense(output_size, activation='sigmoid')(h)
# connecte a h
autoencoder = Model(inputs=x, outputs=r)
autoencoder.compile(optimizer='adam', loss='mse',metrics=['accuracy'])

epochs = 10
batch_size = 32
history = autoencoder.fit(Xtrain, Xtrain, batch_size=batch_size, epochs=epochs, validation_data=(Xtest, Xtest))

#creer un modele uniquement encodeur
conv_encoder = Model(x, h)
# afficher les images encodees avec l'encodeur
encoded_imgs = conv_encoder.predict(Xtest)
n = 10
plt.figure(figsize=(20, 8))
for i in  range(n):
	ax = plt.subplot(1, n, i+1)
	plt.imshow(encoded_imgs[i].reshape(4, 16).T)
	plt.gray()
	ax.get_xaxis().set_visible(False)
	ax.get_yaxis().set_visible(False)
plt.show()


# Prediction du test_set
decoded_imgs = autoencoder.predict(Xtest)
n = 10
plt.figure(figsize=(20, 6))
for i in  range(n):
# Afficher les originaux
	ax = plt.subplot(3, n, i+1)
	plt.imshow(Xtest[i].reshape(28, 28))
	plt.gray()
	ax.get_xaxis().set_visible(False)
	ax.get_yaxis().set_visible(False)
# Afficher la reconstruction
	ax = plt.subplot(3, n, i+n+1)
	plt.imshow(decoded_imgs[i].reshape(28, 28))
	plt.gray()
	ax.get_xaxis().set_visible(False)
	ax.get_yaxis().set_visible(False)
plt.show()