#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Oct  2 23:08:40 2021

@author: isaac
"""

import numpy as np
from FunTareaNumeros import *
from tensorflow.keras.utils import to_categorical
import scipy.io
from sklearn.model_selection import train_test_split
import random

random_seed=random.seed(0)


P = scipy.io.loadmat('NumerosManuscritos.mat')['P'].T
labels=np.array([x//500 for x in range((9+1)*500)])
X_train, X_test, train_labels, test_labels = train_test_split(P, labels, test_size=0.3, 
                                                    random_state=random_seed)

# x,y,z=train_X.shape
# train_X=train_X.reshape(x,y*z)
# test_X=test_X.reshape(test_X.shape[0],y*z)

y_train = to_categorical(train_labels)
y_test = to_categorical(test_labels)

network= MLP (X_train)
plotNN(network,X_train, X_test, y_train, y_test)
Accuracy=Adaline(P.T)

