#!/usr/bin/env python3   # -*- coding: utf-8 -*-
"""
Created on Wed Sep  8
@author: saksevul
"""



import os
import numpy as np
import matplotlib.pyplot as plt

import tensorflow as tf
#from sklearn.model_selection import train_test_split
from tensorflow.keras import layers, Sequential, losses, metrics
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.layers import Conv2D, BatchNormalization, Activation
from tensorflow.keras.optimizers import Adam, SGD



# Ruta a los datos y las clases
dataPath = '/home/saksevul/T/OpenData/'


# Lista de los AODs dentro de path
aod = ['Jet', 'MultiJet']

# Destila los directorios necesarios
#for i in os.listdir(dataPath):
#    if i[-6:] == '_20000':
#        aod.append(i[:-6])

# Ordensmos alfabéticamente
aod.sort()

# Lista de archivos con datos
eFile = []
hFile = []

cal = 'ECal'

# Encuentra los archivos de datos contenidos en los directorios
for i in os.listdir(dataPath+aod[0]+'_20000/'):
    if i[:5]=='ECal'+'-' and i[-4:]=='.txt':
        eFile.append(i[5:9])
    if i[:5]=='HCal'+'-' and i[-4:]=='.txt':
        hFile.append(i[5:9])



# Porción de datos de entrenamiento
p = 0.8

# Definimos el conjunto de datos para el ECal, así como sus clases
for i in range(len(aod)):
    print(aod[i])
    for j in range(len(eFile)):
        if i==0 and j==0:
            eData = np.loadtxt(dataPath+aod[i]+'_20000/'+'ECal'+'-'+eFile[j]+'.txt')
            hData = np.loadtxt(dataPath+aod[i]+'_20000/'+'HCal'+'-'+hFile[j]+'.txt')
            rows = eData.shape[0]
            classes = np.ones(rows)*0
            # Indices del conjunto de entrenamiento y prueba del primer conjunto de AODs
            trainInd = np.random.choice(np.array(range(len(eFile)*rows)),int(p*len(eFile)*rows),replace=False)
            testInd = np.setdiff1d(np.array(range(len(eFile)*rows)),trainInd)
        else:
            eData = eval('np.concatenate((hData,np.loadtxt('+"'"+dataPath+aod[i]+'_20000/'+'ECal'+'-'+eFile[j]+'.txt'+"'"+')), axis=0)')
            hData = eval('np.concatenate((hData,np.loadtxt('+"'"+dataPath+aod[i]+'_20000/'+'HCal'+'-'+hFile[j]+'.txt'+"'"+')), axis=0)')
            classes = eval('np.concatenate((classes,np.ones(rows)*i), axis=0)')
    if i>0:
        trainInd = np.concatenate((trainInd, i*len(eFile)*rows+trainInd[:int(p*len(eFile)*rows)]), axis=0)
        testInd = np.concatenate((testInd, i*len(eFile)*rows+testInd[:int(((10-10*p)/10)*len(eFile)*rows)]), axis=0)


data = eData


# Mexclar una última vez los índices
trainInd = np.random.choice(trainInd,trainInd.shape[0],replace=False)
testInd = np.random.choice(testInd,testInd.shape[0],replace=False)




# Generar las clases en forma categórica
catClasses = to_categorical(classes)



# Creamos los conjuntos de entrenamiento y prueba, de forma ordenada
#trainInd = []
#testInd = []

# Llenar los índices de entrenamiento y prueba en orden 0, 1, 2, 3.
#for i in range(int(data.shape[0]/len(aod))):
#    if i < int(p*data.shape[0]/len(aod)):
#        for j in range(len(aod)):
#            trainInd.append(int(i+j*data.shape[0]/len(aod)))
#    else:
#        for j in range(len(aod)):
#            testInd.append(int(i+j*data.shape[0]/len(aod)))

# Convertir las listas de índices en arreglos de numpy
#trainInd = np.array(trainInd)
#testInd = np.array(testInd)

## Barajear los datos
#trainInd = np.random.choice(trainInd,trainInd.shape[0],replace=False)
#testInd = np.random.choice(testInd,testInd.shape[0],replace=False)




###############################################################################
###############################################################################
###############################################################################

                    #### Perceptrón multicapa ####

# Numero de neuronas iniciales
if cal=='HCal':
    n = 72*34
elif cal=='ECal':
    #n = 360*170
    n = 72*34

# Definición de la red neuronal
network=Sequential()
network.add(layers.Dense(n, activation='relu', input_shape=(data.shape[1],)))
network.add(layers.Dense(n, activation='selu'))
network.add(layers.Dense(int(n/4), activation='elu'))
network.add(layers.Dense(int(n/16), activation='selu'))
network.add(layers.Dense(int(n/64), activation='relu'))
network.add(layers.Dense(len(aod), activation='softmax'))



# Compilar la red
network.compile(
    optimizer=SGD(learning_rate=0.1),
    loss=losses.binary_crossentropy,
    metrics=[metrics.Precision(), metrics.Recall()])



# Correr la red
history = network.fit(
    data[trainInd],
    catClasses[trainInd],
    epochs=24,
    batch_size=56,
    validation_split=0.1,
    verbose=1)

######
#           ¿Puedo ver la matriz de confución para los datos de validaciós?
######




# Variable para extraer las métricas del entrenamiento
history_dict = history.history
history_dict.keys()


# Grafica los valores obtenidos en cada
epochs = range(1, len(history_dict['loss']) + 1)
plt.figure(figsize=(16,8))
plt.plot(epochs, history_dict['loss'], 'ko', label='Training loss')
plt.plot(epochs, history_dict['recall'], 'rv', label='Training recall')
plt.plot(epochs, history_dict['precision'], 'b^', label='Training precision')
#plt.plot(epochs, history_dict['val_loss'], 'k', label='Validation loss')
plt.plot(epochs, history_dict['val_recall'], 'r', label='Validation recall')
plt.plot(epochs, history_dict['val_precision'], 'b', label='Validation precision')
plt.title('Training and Test loss, precision and recall')
#plt.suptitle('Dense NN')
plt.xlabel('Epochs')
plt.ylabel('Loss, Precision & Recall')
plt.legend()
plt.show()




results = network.evaluate(data[testInd], catClasses[testInd])
print("test loss, test acc:", results)


clasesPred = network.predict_classes(data[testInd])
matrix = tf.math.confusion_matrix(classes[testInd], clasesPred)
print(matrix)



###############################################################################
###############################################################################
###############################################################################

#CNN

# Tamaño de las imágenes
if cal=='HCal':
    x = 72
    y = 34
elif cal=='ECal':
    #x = 360
    #y = 170
    x = 72
    y = 34


# Transformar los datos a imágenes (matrices)
dataTrain2D = np.reshape(data[trainInd],(trainInd.shape[0], x, y,1))
dataTest2D = np.reshape(data[testInd],(testInd.shape[0], x, y,1))


# Numero de kernels por capa
k1 = 16
k2 = 32
k3 = 8
k4 = 4

# Red CNN
model=Sequential()
# Por que se les ponen nombre?
model.add(Conv2D(k1, (3, 3), name = 'conv0', activation='relu',input_shape = (x, y, 1)))
# Función exacta del BatchN
model.add(BatchNormalization(axis = 3, name = 'bn0'))
model.add(Activation('relu'))
model.add(layers.AveragePooling2D((2, 3)))

model.add(layers.Conv2D(k2, (3, 3), activation='relu'))
model.add(BatchNormalization(axis = 3, name = 'bn1'))
model.add(Activation('relu'))
model.add(layers.AveragePooling2D((2, 2)))
# Integrar una nueva capa


# Tamaño del vector de entrada:
k = k1*k2*k3*k4*(16*4)

model.add(layers.Flatten())
model.add(layers.Dense(k, activation='tanh'))
model.add(layers.Dense(4*k, activation='relu'))
model.add(layers.Dense(int(k/8), activation='sigmoid'))
model.add(layers.Dense(int(k/32), activation='relu'))
model.add(layers.Dense(len(aod), activation='softmax'))
print(model.summary())


model.compile(optimizer=Adam(lr=.001),
                loss=losses.binary_crossentropy,
                  metrics=[metrics.binary_accuracy,metrics.Precision(),
                            metrics.Recall(),
                            metrics.TrueNegatives(),
                            metrics.FalsePositives()])



history=model.fit(dataTrain2D,
    catClasses[trainInd],
    epochs=24,
    batch_size=512,
    validation_split=0.1,
    verbose=1)



history_dict = history.history
history_dict.keys()


# Grafica los valores obtenidos en cada
epochs = range(1, len(history_dict['loss']) + 1)
plt.figure(figsize=(16,8))
plt.plot(epochs, history_dict['loss'], 'ko', label='Training loss')
plt.plot(epochs, history_dict['recall_1'], 'rv', label='Training recall')
plt.plot(epochs, history_dict['precision_1'], 'b^', label='Training precision')
#plt.plot(epochs, history_dict['val_loss'], 'k', label='Validation loss')
plt.plot(epochs, history_dict['val_recall_1'], 'r', label='Validation recall')
plt.plot(epochs, history_dict['val_precision_1'], 'b', label='Validation precision')
plt.title('Training and Test loss, precision and recall')
#plt.suptitle('Dense NN')
plt.xlabel('Epochs')
plt.ylabel('Loss, Precision & Recall')
plt.legend()
plt.show()





results = model.evaluate(dataTest2D, catClasses[testInd])
print("test loss, test acc:", results)


clasesPred = model.predict_classes(dataTest2D)
matrix = tf.math.confusion_matrix(classes[testInd], clasesPred)
print(matrix)












#
