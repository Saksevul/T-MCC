#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jun 15 18:53:52 2021

@author: isaac
"""
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow as tf
from tensorflow.keras import layers, models, Sequential, losses, metrics
from keras.datasets import mnist
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.optimizers import Adam

# load dataset
(trainX, train_labels), (testX, test_labels) = mnist.load_data()

# for i in range(10):
#     plt.figure()
#     plt.imshow(testX[i])
#     plt.show()

#X_train=X_train.reshape(x,y,1)
y_train = to_categorical(train_labels)
y_test = to_categorical(test_labels)
X_train=[]
X_test=[]

for i in range(len(trainX)):
    X_train.append(trainX[i].flatten())
    
for i in range(len(testX)):
    X_test.append(testX[i].flatten())

X_train=np.array(X_train)
X_test=np.array(X_test)

network=Sequential()
network.add(layers.Dense(512,activation='relu',input_shape=(X_train.shape[1],)))
network.add(layers.Dense(128, activation='sigmoid'))
network.add(layers.Dense(128, activation='relu'))
network.add(layers.Dense(128, activation='tanh'))
network.add(layers.Dense(64, activation='relu'))
network.add(layers.Dense(32, activation='relu'))
network.add(layers.Dense(10, activation='softmax'))

network.compile(optimizer=Adam(lr=.001),
                loss=losses.binary_crossentropy,
                  metrics=[metrics.binary_accuracy,metrics.Precision(), 
                            metrics.Recall(),
                            metrics.TrueNegatives(),
                            metrics.FalsePositives()])

history=network.fit(X_train,
    y_train,
    epochs=100,
    batch_size=128,
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

epochs = range(1, len(loss_values) + 1)
plt.figure(figsize=(10,8))
plt.plot(epochs, loss_values, 'bo', label='Training loss')
plt.plot(epochs, val_loss_values, 'b', label='Validation loss')
plt.plot(epochs, acc, 'ro', label='Training acc')
plt.plot(epochs, val_acc, 'r', label='Validation acc')
plt.plot(epochs, prec, 'go', label='Training precision')
plt.plot(epochs, val_prec, 'g', label='Validation precision')
plt.plot(epochs, recall, 'ko', label='Training recall')
plt.plot(epochs, val_recall, 'k', label='Validation recall')
plt.title('Training and validation loss, accurracy, precision and recall')
plt.suptitle('Dense NN')
plt.xlabel('Epochs')
plt.ylabel('Loss & Accuracy , Precision & Recall')
plt.legend()
plt.show()

