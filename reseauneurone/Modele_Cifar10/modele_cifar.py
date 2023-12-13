from tensorflow import keras
from tensorflow.keras import optimizers
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Conv2D, MaxPooling2D, Flatten

from tensorflow.keras.datasets import cifar10

(X_train_data, Y_train_data), (X_test_data, Y_test_data) = cifar10.load_data()
assert X_train_data.shape == (50000, 32, 32, 3)
assert X_test_data.shape == (10000, 32, 32, 3)
assert Y_train_data.shape == (50000, 1)
assert Y_test_data.shape == (10000, 1)

num_classes = 10 
labels = ['airplane','automobile','bird','cat',
          'deer','dog','frog','horse','ship','truck']

#print(Y_train_data)
''' Un tab de 10 000 avec des chiffres de 0 à 9 pour 
    qu'ensuite on interprete avec labels pour trouver le nom '''

Y_train = keras.utils.to_categorical(Y_train_data, num_classes)
#X_train = X_train_data.reshape(50000,32*32*3)
X_train = X_train_data.astype('float32')
X_train = X_train/255

Y_test = keras.utils.to_categorical(Y_test_data, num_classes)
#X_test = X_test_data.reshape(10000,32*32*3)
X_test = X_test_data.astype('float32')
X_test = X_test/255

modele = Sequential()

p=128

modele.add(Conv2D(64, kernel_size=3, padding='same', activation='relu', input_shape=(32,32,3)))
modele.add(Conv2D(64, kernel_size=3, padding='same', activation='relu'))
modele.add(MaxPooling2D(pool_size=(2,2)))
modele.add(Conv2D(64, kernel_size=3, padding='same', activation='relu'))
modele.add(MaxPooling2D(pool_size=(2,2)))
modele.add(Conv2D(64, kernel_size=3, padding='same', activation='relu'))
modele.add(Flatten())

'''modele.add(Dense(p, input_dim=32*32*3, activation='relu'))
modele.add(Dense(p, activation='relu'))
modele.add(Dense(p, activation='relu'))
modele.add(Dense(p, activation='relu')) '''

modele.add(Dense(10, activation='softmax'))


mysgd = optimizers.SGD(lr=0.1, momentum=0.9, nesterov=True)
modele.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])

modele.fit(X_train,Y_train, epochs=10, batch_size=32)

modele.save('modelecifar.h5')

loss, accuracy = modele.evaluate(X_test,Y_test)

print ("Loss: ", loss)
print ("Accuracy: ", accuracy)





