#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Oct  3 23:23:05 2021

@author: isaac
"""

import numpy as np
import scipy.io
from sklearn.model_selection import train_test_split
import random
from tensorflow.keras.utils import to_categorical
def sigmoid(x):
    ''' Sigmoid like function using tanh '''
    return np.tanh(x)


def dsigmoid(x):
    ''' Derivative of sigmoid above '''
    return 1.0-x**2


class Elman:
    ''' Elamn network '''


    def __init__(self, *args):
        ''' Initialization of the perceptron with given sizes.  '''

        self.shape = args
        n = len(args)

        # Build layers
        self.layers = []

        # Input layer (+1 unit for bias
        #              +size of first hidden layer)
        self.layers.append(np.ones(self.shape[0]+1+self.shape[1]))
        #print(self.layers)

        # Hidden layer(s) + output layer
        for i in range(1,n):
            self.layers.append(np.ones(self.shape[i]))

        # Build weights matrix
        self.weights = []
        for i in range(n-1):
            self.weights.append(np.zeros((self.layers[i].size,
                                          self.layers[i+1].size)))

        # dw will hold last change in weights (for momentum)
        self.dw = [0,]*len(self.weights)

        # Reset weights
        self.reset()


    def reset(self):
        ''' Reset weights '''

        for i in range(len(self.weights)):
            Z = np.random.random((self.layers[i].size,self.layers[i+1].size))
            self.weights[i][...] = (2*Z-1)*0.25


    def propagate_forward(self, data):
        ''' Propagate data from input layer to output layer. '''

        # Set input layer with data
        self.layers[0][:self.shape[0]] = data
        # and first hidden layer
        self.layers[0][self.shape[0]:-1] = self.layers[1]

        # Propagate from layer 0 to layer n-1 using sigmoid as activation function
        for i in range(1,len(self.shape)):
            # Propagate activity
            self.layers[i][...] = sigmoid(np.dot(self.layers[i-1],self.weights[i-1]))

        # Return output
        return self.layers[-1]


    def propagate_backward(self, target, lrate=0.1, momentum=0.1):
        ''' Back propagate error related to target using lrate. '''

        deltas = []

        # Compute error on output layer
        error = target - self.layers[-1]
        delta = error*dsigmoid(self.layers[-1])
        deltas.append(delta)

        # Compute error on hidden layers
        for i in range(len(self.shape)-2,0,-1):
            delta = np.dot(deltas[0],self.weights[i].T)*dsigmoid(self.layers[i])
            deltas.insert(0,delta)
            
        # Update weights
        for i in range(len(self.weights)):
            layer = np.atleast_2d(self.layers[i])
            delta = np.atleast_2d(deltas[i])
            dw = np.dot(layer.T,delta)
            self.weights[i] += lrate*dw + momentum*self.dw[i]
            self.dw[i] = dw

        # Return error
        return (error**2).sum()


# -----------------------------------------------------------------------------
if __name__ == '__main__':
    import matplotlib
    import matplotlib.pyplot as plt
    
    random_seed=random.seed(0)
    P = scipy.io.loadmat('NumerosManuscritos.mat')['P'].T
    labels=np.array([x//500 for x in range((9+1)*500)])
    _, X_test, _, test_labels = train_test_split(P, labels, test_size=0.1, 
                                                        random_state=random_seed)
        
    y_train = to_categorical(labels)
    y_test = to_categorical(test_labels)
    network = Elman(P.shape[1],8,len(np.unique(labels)))
    samples = np.zeros(len(np.unique(labels)), 
                       dtype=[('input',  float, P.shape[1]), 
                              ('output', float, len(np.unique(labels)))])
    for i in range(len(np.unique(labels))):
        samples[i]  = np.mean(P[i*500:(i+1)*500],axis=0), y_train[i*500]
        # samples[i]  = P[i*520], y_train[i*500]
        plt.figure()
        plt.imshow(samples['input'][i].reshape((20,20)))
        plt.show()


    for i in range(10000):
        n = i%samples.size
        network.propagate_forward(samples['input'][n])
        network.propagate_backward(samples['output'][n],i)
    
    cont=0
    for i in range(y_test.shape[0]):
        o = network.propagate_forward( X_test[i] )
        if np.argmax(o)==test_labels[i]:
            cont+=1
        # print('Sample %d: %s->%s' % (i,test_labels[i],y_test[i]))
        # print('               Network output: %s' % (o == o.max()).astype(float))
        # print 
    
    print('Acurracy=' +str(cont/len(test_labels)))