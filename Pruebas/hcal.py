#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep
@author: saksevul
"""




import matplotlib.pyplot as plt

import pandas as pd
import tensorflow as tf

import os
from sklearn.model_selection import train_test_split
from tensorflow.keras import layers, models, Sequential, losses, metrics
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.layers import Conv2D, BatchNormalization, Activation
from tensorflow.keras.utils import to_categorical
import numpy as np



# Ruta a los datos y las clases
path = '/home/saksevul/T/OpenData/'



# Lista de los elementos dentro del directorio path
files=os.listdir(path)
file = []



# Destila los directorios necesarios
for i in range(len(files)):
    if files[i][-5:] == '20000':
        file.append(files[i])



# Cargar archivos de forma exquisita. Reduce drásticamente las líneas de código
for i in file:
    exec('HCal'+i[:3]+'Test = np.loadtxt('+"'"+path+i+'/'+'HCal-0002.txt'+"'"+')')
    exec('HCal'+i[:3]+'Train = np.loadtxt('+"'"+path+i+'/'+'HCal-0001.txt'+"'"+')')



# Concatenar
HCalMin = eval('np.concatenate((HCal'+file[0][:3]+'Train,HCal'+file[0][:3]+'Test))')

HCalJet = eval('np.concatenate((HCal'+file[1][:3]+'Train,HCal'+file[1][:3]+'Test))')

HCalMul = eval('np.concatenate((HCal'+file[2][:3]+'Train,HCal'+file[2][:3]+'Test))')

HCalBTa = eval('np.concatenate((HCal'+file[3][:3]+'Train,HCal'+file[3][:3]+'Test))')



# Vector de clases
ClasesMin = np.ones(HCalJet.shape[0])*0
ClasesJet = np.ones(HCalJet.shape[0])
ClasesMul = np.ones(HCalJet.shape[0])*2
ClasesBTa = np.ones(HCalJet.shape[0])*3



# Variables para ingresar a la red
datosTest = []
datosTrain = []
clasesTest = []
clasesTrain = []



# Crear una sola matriz para los datos y las Clases
# Juntar, barajear y separar
X_test, X_train, Y_test, Y_train = train_test_split(HCalJet,ClasesJet,test_size=0.7);
datosTest.append(X_test)
datosTrain.append(X_train)
clasesTest.append(Y_test)
clasesTrain.append(Y_train)

X_test, X_train, Y_test, Y_train = train_test_split(HCalMin,ClasesMin,test_size=0.7);
datosTest.append(X_test)
datosTrain.append(X_train)
clasesTest.append(Y_test)
clasesTrain.append(Y_train)

X_test, X_train, Y_test, Y_train = train_test_split(HCalBTa,ClasesBTa,test_size=0.7);
datosTest.append(X_test)
datosTrain.append(X_train)
clasesTest.append(Y_test)
clasesTrain.append(Y_train)

X_test, X_train, Y_test, Y_train = train_test_split(HCalMul,ClasesMul,test_size=0.7);
datosTest.append(X_test)
datosTrain.append(X_train)
clasesTest.append(Y_test)
clasesTrain.append(Y_train)



# Ordenar los datos y las clases para ser entregadas a la red
datosTest = np.array(datosTest)
datosTest=datosTest.reshape((int(datosTest.shape[0]*datosTest.shape[1]),datosTest.shape[2]))

datosTrain = np.array(datosTrain)
datosTrain=datosTrain.reshape((int(datosTrain.shape[0]*datosTrain.shape[1]),datosTrain.shape[2]))

clasesTest = np.array(clasesTest)
clasesTest=clasesTest.reshape((int(clasesTest.shape[0]*clasesTest.shape[1]),1))

clasesTrain = np.array(clasesTrain)
clasesTrain=clasesTrain.reshape((int(clasesTrain.shape[0]*clasesTrain.shape[1]),1))


# Reordenar los conjuntos
indexTrain = np.random.choice(datosTrain.shape[0],datosTrain.shape[0],replace=False)
indexTest = np.random.choice(datosTest.shape[0],datosTest.shape[0], replace=False)



# Datos "desordenados"
datosTrain = datosTrain[indexTrain]
datosTest = datosTest[indexTest]

clasesTrain = clasesTrain[indexTrain]
clasesTest = clasesTest[indexTest]



# Cambiar las clases a categorias
clasesEntrenamiento = to_categorical(clasesTrain)
clasesPrueba = to_categorical(clasesTest)
 


###############################################################################
###############################################################################
###############################################################################

                    #### Perceptrón multicapa ####



# Definición de la red neuronal
network=Sequential()
network.add(layers.Dense(4096,activation='relu',input_shape=(datosTrain.shape[1],)))
network.add(layers.Dense(2048, activation='sigmoid'))
network.add(layers.Dense(512, activation='relu'))
network.add(layers.Dense(64, activation='tanh'))
network.add(layers.Dense(16, activation='relu'))
network.add(layers.Dense(4, activation='softmax'))



# Compilar la red
network.compile(optimizer=Adam(lr=0.001),
                loss=losses.binary_crossentropy,
                # Métricas para conocer que tan bueno es el ajuste 
                metrics=[metrics.binary_accuracy,
                         metrics.Precision(), 
                         metrics.Recall(),
                         metrics.TrueNegatives(),
                         metrics.FalsePositives()])



# correr la red
history=network.fit(
    datosTrain,
    clasesEntrenamiento,
    epochs=100,
    batch_size=128,
    validation_split=0.1,
    verbose=1)



# Variable para extraer las métricas del entrenamiento
history_dict = history.history
history_dict.keys()



# Extrae las métricas del entrenamiento de la red
loss_values = history_dict['loss']
val_loss_values = history_dict['val_loss']
acc = history_dict['binary_accuracy']
val_acc = history_dict['val_binary_accuracy']
prec = history_dict['precision']
val_prec = history_dict['val_precision']
recall = history_dict['recall']
val_recall = history_dict['val_recall']



# Grafica los valores obtenidos en cada 
epochs = range(1, len(loss_values) + 1)
plt.figure(figsize=(16,8))
plt.plot(epochs, loss_values, 'ko', label='Training loss')
plt.plot(epochs, val_loss_values, 'k', label='Validation loss')
#plt.plot(epochs, acc, 'go', label='Training acc')
#plt.plot(epochs, val_acc, 'g', label='Validation acc')
plt.plot(epochs, prec, 'b^', label='Training precision')
plt.plot(epochs, val_prec, 'b', label='Validation precision')
plt.plot(epochs, recall, 'rv', label='Training recall')
plt.plot(epochs, val_recall, 'r', label='Validation recall')
plt.title('Training and validation loss, precision and recall')
#plt.suptitle('Dense NN')
plt.xlabel('Epochs')
plt.ylabel('Loss, Precision & Recall')
plt.legend()
plt.show()




results = network.evaluate(datosTest, clasesPrueba)
print("test loss, test acc:", results)

   
clasesPred = network.predict_classes(datosTest)
matrix = tf.math.confusion_matrix(clasesTest, clasesPred)
print(matrix)



###############################################################################
###############################################################################
###############################################################################

#CNN

# Tamaño de las imágenes
x = 72
y = 34


# Transformar los datos a imágenes (matrices)
datosTrain2D = np.reshape(datosTrain,(datosTrain.shape[0], x, y,1))
datosTest2D = np.reshape(datosTest,(datosTest.shape[0], x, y,1))



# Red CNN
model=Sequential()
model.add(Conv2D(8, (3, 3), strides = (3, 3), name = 'conv0', activation='relu',input_shape = (x, y, 1)))
model.add(BatchNormalization(axis = 3, name = 'bn0'))
model.add(Activation('relu'))
model.add(layers.MaxPooling2D((2, 2)))
model.add(layers.Conv2D(8, (3, 3), activation='relu'))
model.add(BatchNormalization(axis = 3, name = 'bn1'))
model.add(Activation('relu'))
model.add(layers.MaxPooling2D((2, 2)))
# Integrar una nueva capa

model.add(layers.Flatten())
model.add(layers.Dense(256, activation='tanh'))
model.add(layers.Dense(128, activation='relu'))
model.add(layers.Dense(64, activation='sigmoid'))
model.add(layers.Dense(16, activation='relu'))
model.add(layers.Dense(4, activation='softmax'))
print(model.summary())

          
model.compile(optimizer=Adam(lr=.001),
                loss=losses.binary_crossentropy,
                  metrics=[metrics.binary_accuracy,metrics.Precision(), 
                            metrics.Recall(),
                            metrics.TrueNegatives(),
                            metrics.FalsePositives()])



history=model.fit(datosTrain2D,
    clasesEntrenamiento,
    epochs=100,
    batch_size=512,
    validation_split=0.1,
    verbose=1)



history_dict = history.history
history_dict.keys()
loss_values = history_dict['loss']
val_loss_values = history_dict['val_loss']
acc = history_dict['binary_accuracy']
val_acc = history_dict['val_binary_accuracy']
prec = history_dict['precision']
val_prec = history_dict['val_precision']
recall = history_dict['recall']
val_recall = history_dict['val_recall']





# Grafica los valores obtenidos en cada 
epochs = range(1, len(loss_values) + 1)
plt.figure(figsize=(16,8))
plt.plot(epochs, loss_values, 'ko', label='Training loss')
plt.plot(epochs, val_loss_values, 'k', label='Validation loss')
#plt.plot(epochs, acc, 'go', label='Training acc')
#plt.plot(epochs, val_acc, 'g', label='Validation acc')
plt.plot(epochs, prec, 'b^', label='Training precision')
plt.plot(epochs, val_prec, 'b', label='Validation precision')
plt.plot(epochs, recall, 'rv', label='Training recall')
plt.plot(epochs, val_recall, 'r', label='Validation recall')
plt.title('Training and validation loss, precision and recall')
#plt.suptitle('Dense NN')
plt.xlabel('Epochs')
plt.ylabel('Loss, Precision & Recall')
plt.legend()
plt.show()





results = model.evaluate(datosTest2D, clasesPrueba)
print("test loss, test acc:", results)

   
clasesPred = model.predict_classes(datosTest2D)
matrix = tf.math.confusion_matrix(clasesTest, clasesPred)
print(matrix)












#