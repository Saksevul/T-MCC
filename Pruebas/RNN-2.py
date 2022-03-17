#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Aug 11 14:57:38 2021

@author: saksevul
"""
import NeuralNet
import PyNeurGen

#   Standard Multy Layer Perceptron with two hidden layers
input_nodes = 2
hidden_nodes1 = 2
hidden_nodes2 = 3
output_nodes = 1

net = NeuralNet()
net.init_layers(
    input_nodes,
    [hidden_nodes1,
    hidden_nodes2],
    output_nodes)

net.randomize_network()


#   Jordan style recurrent network
input_nodes = 2
hidden_nodes = 2
output_nodes = 1
existing_weight_factor = .7
net = NeuralNet()

net.init_layers(
    input_nodes,
    [hidden_nodes],
    output_nodes,
    JordanRecurrent(existing_weight_factor))

net.randomize_network()