#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Oct  2 23:15:30 2021

@author: isaac
"""


import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import models
from tensorflow.keras import layers
from tensorflow.keras.layers import Input, Dense, Activation, BatchNormalization, Flatten, Conv2D
from tensorflow.keras.layers import AveragePooling2D, MaxPooling2D, Dropout, GlobalAveragePooling2D
from tensorflow.keras.optimizers import Adam
from tensorflow.keras import optimizers
from tensorflow.keras import losses
from tensorflow.keras import metrics
from tensorflow.keras.utils import to_categorical
from tensorflow.keras import Model
import scipy.io
import numpy as np
import matplotlib.pyplot as plt
import time

def MLP (train_X):
    
    network = models.Sequential()
    network.add(Dense(512, activation='relu', input_shape=(train_X.shape[1],)))
    network.add(Dense(128, activation='sigmoid'))
    network.add(Dense(128, activation='relu'))
    network.add(Dense(128, activation='tanh'))
    network.add(Dense(64, activation='relu'))
    network.add(Dense(32, activation='relu'))
    network.add(Dense(10, activation='softmax'))
    
    network.compile(optimizer=Adam(learning_rate=.001),
                      loss=losses.binary_crossentropy,
                      metrics=[metrics.binary_accuracy,
                               tf.keras.metrics.Precision(), 
                               tf.keras.metrics.Recall()])

    return network

def Adaline(P):
    Q=P.shape[1]
    Z=np.concatenate((P,np.ones((1,Q),int)))
    T=np.concatenate((np.ones((1,500)),np.ones((9,500))*-1))
    for i in np.arange (1,10):
        T=np.concatenate((T,np.concatenate((np.ones((i,500))*-1,
                                          np.ones((1,500)),
                                          np.ones((9-i,500))*-1))),axis=1)
        
    
    R = 1/Q*Z.dot(Z.T) 
    H = 1/Q*Z.dot(T.T)
    Xmin=np.linalg.pinv(R).dot(H)
    W = Xmin[0:400,:].T
    b = Xmin[400,:].T
    a=np.zeros(Q)
    iwin=np.zeros(Q)
    
    for q in range(Q):
        a[q]=np.max(W.dot(P[:,q])+b)
        iwin[q]=np.argmax(W.dot(P[:,q])+b)
        
    y =np.zeros(500)
    
    for i in np.arange (1,10):
        y=np.concatenate((y,np.ones(500)*i))
        
    NumeroAciertos = sum(y==iwin)
    PorcentajeAciertos = NumeroAciertos/5000*100
    return PorcentajeAciertos

def plotNN(network,train_X, test_X, y_train, y_test):
    
    history=network.fit(
        train_X,
        y_train,
        epochs=100,
        batch_size=128,
        validation_split=0.1,
        verbose=1
        )
    
    results = network.evaluate(test_X, y_test, batch_size=128)
    loss, acc, precision_new, recall_new=results
    print("test loss, test acc , test precision, test recall:", results)
     
    history_dict = history.history
    keys=list(history_dict.keys())
    loss_values = history_dict[keys[0]]
    val_loss_values = history_dict[keys[4]]
    acc = history_dict[keys[1]]
    val_acc = history_dict[keys[5]]
    prec = history_dict[keys[2]]
    val_prec = history_dict[keys[6]]
    recall = history_dict[keys[3]]
    val_recall = history_dict[keys[7]]
    
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
    plt.suptitle('Parallel NN')
    plt.xlabel('Epochs')
    plt.ylabel('Loss & Accuracy , Precision & Recall')
    plt.legend()
    plt.show()