#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Oct  3 11:57:29 2021

@author: saksevul
"""

import time
import random
import scipy.io
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
import tensorflow as tf
from tensorflow import keras
from keras.datasets import mnist
from tensorflow.keras import layers, losses, metrics, Model, models, optimizers
from tensorflow.keras.layers import Input, Dense, Activation, BatchNormalization, Flatten, Conv2D
#from tensorflow.keras.layers import AveragePooling2D, MaxPooling2D, Dropout, GlobalAveragePooling2D
from tensorflow.keras.optimizers import Adam, SGD
from tensorflow.keras.utils import to_categorical


# sigmoide
def f0(x):
    x = np.array(x)
    #print(x.shape, x)
    for i in range(x.shape[0]):
        for j in range(x.shape[1]):
            x[i][j] = 1.0/(1.0+np.exp(-x[i][j]))
    return np.matrix(x)

def df0(x):
    # x.shape = (100,1)
    F = np.zeros((x.shape[0],x.shape[0]))
    for i in range(x.shape[0]):
        F[i][i] = (1.0-x[i][0])*x[i][0]
    return np.matrix(F)




def df00(x):
    diag = 1.0
    x = np.array(x)
    # x.shape = (100,1)
    for i in range(x.shape[0]):
        diag *= (1.0-x[i][0])*x[i][0]
    return diag


#lineal
def f1(x):
    return x

def df1(x):
    return 1.0


# tanh
def f2(x):
    x = np.array(x)
    #print(x)
    for i in range(x.shape[0]):
        for j in range(x.shape[1]):
            x[i][j] = (np.exp(x[i][j])-np.exp(-x[i][j]))/(np.exp(x[i][j])+np.exp(-x[i][j]))
    #print(x)
    #print()
    return np.matrix(x)

def df2(x):
    diag = 1.0
    x = np.array(x)
    # x.shape = (400,1)
    for i in range(x.shape[0]):
        diag *= (1-x[i][0]**2)
    return diag



def perceptronMC(data, t, lay, epochs=60):
    # parametro alpha
    alpha = 0.1
    # numero de capas, M
    M = len(lay)
    # x.shape = (4000,400)
    Q = data.shape[0]
    m = [data.shape[1]]
    for i in range(M):
        m.append(lay[i])

    #print(M, m)

    # iniciaizar los valores para las matrices
    W = []
    b = []
    # llenar los valores en función del tamaño de las capas
    for i in range(M):
        # dimensión de m_i x m_{i+1}. comienza con índice 0
        W.append(np.matrix(np.random.rand(m[i],m[i+1]))*2-1)
        # b[i] tien dimnsion igual a la cantidad de neuronas de la capa i
        b.append(np.matrix(np.random.rand(m[i+1],1))*2-1)

    # print(W[0], b[0])

    # cilo para el número de datos
    for epoch in range(epochs):
        e = []
        error = 0
        total = 0
        # para el numero de datos, Q
        for q in range(Q):

            # propagación
            # dimensión de a_i, b_i, n_i y s_i = m_i x 1
            a = [np.matrix(data[q]).T]
            n = []
            s = []
            # ciclo para todas las capas, M
            for m in range(M):
                n.append(W[m].T*a[m]+b[m])
                a.append(f0(n[m]))
                s.append(0)
                if q % 1000 == 0 and m==2:
                #    print(q, m, 'n', n[m].T)
                    print('  ', q, m, 'a', a[m+1].T)
                    print('  ', q, m, 't', np.matrix(t[q]))
                    print()

            # retropropagación
            e.append(np.matrix(t[q]).T - a[M])
            s[M-1] = -2.0*df0(a[M])*e[q]
            #s[M-1] = -2.0*df0(a[M-1])*e[q]
            #if q % 100 == 0:
                #print(q, 's', s[M-1].T, df0(a[M-1]), a[M-1].T)

            #print(e[q].T*e[q])
            
            total += 1
            if np.array(e[q].T*e[q])[0][0] > 0.1:
                #print(np.array(e[q].T*e[q])[0][0], a[M].T)
                error += 1

            # ciclo para todas las capas
            for m in range(M-1,0,-1):
                #print(df(n[m-1]).shape,  W[m].shape,  s[0].shape)
                s[m-1] = df0(a[m])*(W[m]*s[m])
                #s[m-1] = df0(a[m-1])*(W[m]*s[m])

            # actualizar para toda m
            for m in range(M):
                #print(W[m].shape, s[m].shape, a[m].T.shape)
                W[m] = W[m] - alpha*(s[m]*a[m].T).T
                b[m] = b[m] - alpha*s[m]
                #if q % 100 == 0:
                    #print(q, m, 's', s[m].T)
                

        print(str(epoch)+' accuracy = 1 - '+str(error)+'/'+str(total)+' = '+str(1-error/total))
        print()

    # regresa los valores de las matrices, los bias y los errores
    return W, b



def testPreceptron(W, b, data, t, lay, epochs=60):
    # numero de capas, M
    Q = data.shape[0]
    M = len(lay)
    m = [data.shape[1]]
    for i in range(M):
        m.append(lay[i])
    e = []
    error = 0
    total = 0
    # para el numero de datos, Q
    for q in range(Q):

        # propagación
        # dimensión de a_i, b_i, n_i y s_i = m_i x 1
        a = [np.matrix(data[q]).T]
        # ciclo para todas las capas, M
        for m in range(M):
            a.append(W[m].T*a[m]+b[m])

        # retropropagación
        e.append(np.matrix(t[q]).T - a[M])

        total += 1
        if np.array(e[q].T*e[q])[0][0] > 0:
            print(np.array(e[q].T*e[q])[0][0], t[q], a[M].T)
            error = error + 1

    print(' accuracy = 1 - '+str(error)+'/'+str(total)+' = '+str(1-error/total))

    return



# load dataset
#(trainX, train_labels), (testX, test_labels) = mnist.load_data()

# # load dataset
# (trainX, train_labels), (testX, test_labels) = mnist.load_data()
# cargar los datos
data = scipy.io.loadmat('/home/saksevul/Documents/MCC/Cursos/3erSemestre/RNA/Tarea5/NumerosManuscritos.mat')['P'].T
# etiquetas de entrenamiento
labels = np.array([i//500 for i in range(len(data))])
# dividir datos en entrenamiento y prueba
X_train, X_test, train_labels, test_labels = train_test_split(data, labels, test_size=0.2, random_state=random.seed(0))
# pasar las etiquetas a categorias
y_train = to_categorical(train_labels)
y_test = to_categorical(test_labels)

# generar la red perceptron
W, b = perceptronMC(X_train, y_train, [100,25,10])
