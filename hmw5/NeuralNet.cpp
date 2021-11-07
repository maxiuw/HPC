#ifndef NEURAL_NET_CPP
#define NEURAL_NET_CPP

/**
 * A simple neural network implementationi n C++.  This implementation
 * is essentially based on the implementation from Michael Nielsen at
 * http://neuralnetworksanddeeplearning.com/
 *
 * Copyright (C) 2021 raodm@miamiOH.edu
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <functional>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>

#include "NeuralNet.h"

// The constructor to create a neural network with a given number of
// layers, with each layer having a given number of neurons.
NeuralNet::NeuralNet(const std::vector<int>& layers) :
    layerSizes(1, layers.size()) {
    // Copy the values into the layer size matrix
    std::copy_n(layers.begin(), layers.size(), layerSizes[0].begin());
    // Use helper method to initializes matrices to default values.
    initBiasAndWeightMatrices(layers, biases, weights);
}

// Helper method called from the constructor to initialize the biases
// and weight matrices for each layer in the neural netowrk.
void
NeuralNet::initBiasAndWeightMatrices(const std::vector<int>& layerSizes,
                                     MatrixVec& biases,
                                     MatrixVec& weights) const {
    // Optionally use a random number generator to initialize values below.
    // std::uniform_real_distribution<Val> rndDist;
    // std::random_device rndGen;
    // auto rnd = [&](const auto&){ return rndDist(rndGen); };
    
    // Create the column matrices for each layer in the nnet.  Each
    // value is initialized with a random value in the range 0 to 1.0
    for (size_t lyr = 1; (lyr < layerSizes.size()); lyr++) {
        // Convenience variables to keep code readable
        const int rows = layerSizes.at(lyr), cols = layerSizes.at(lyr - 1);

        // biases.push_back(Matrix(rows, 1).apply(rnd));
        biases.push_back(Matrix(rows, 1));

        // Create the 2-D matrices of weights for each layer
        // weights.push_back(Matrix(rows, cols).apply(rnd));
        weights.push_back(Matrix(rows, cols));
    }    
}

// The main learning method that essentially uses matrix operations
// for performing the operations to update weights and biases for each
// layer in the neural network.
void NeuralNet::learn(const Matrix& inputs, const Matrix& expected,
                      const Val eta) {
    // First process the information by feeding inputs through each
    // layer and recording the intermediate results.
    auto activation = inputs;

    // List of matrices to store the deltas and errors for each layer
    MatrixVec activations = { inputs }, zs;

    // Do the forward propagation layer-by-layer
    for (size_t lyr = 0; (lyr < biases.size()); lyr++) {
        zs.push_back(weights[lyr].dot(activation) + biases[lyr]);
        // Update activations for the next layer (i.e., next iteration)
        activation = zs.back().apply(sigmoid);
        // Store activations for each layer for use in backward-pass below.
        activations.push_back(activation);
    }
    
    // ----------------[ Now do the backward pass ]-----------------
    // This pass computes nabla (∇) in weights and biases so that the
    // network can be suitably updated to minimize errors.
    auto delta = (activations.back() - expected) * zs.back().apply(invSigmoid);


    // Create intermediate bias and weights matrices to be updated as
    // part of the back propagation.
    MatrixVec nabla_b, nabla_w;
    // Store the delta for use in the interations below
    nabla_b.push_back(delta);
    const int lastLyr = layerSizes[0].size() - 1;
    nabla_w.push_back(delta.dot(activations.at(lastLyr - 1).transpose()));

    // We propagate the errors backwards (to correct weights and
    // biases), from the outputs back to the inputs. Note that the
    // order of zs and nabla values are from output to input order.
    for (auto lyr = 2; (lyr <= lastLyr); lyr++) {
        const auto sp = zs[lastLyr - lyr].apply(invSigmoid);
        delta = weights[lastLyr - lyr + 1].transpose().dot(delta) * sp;
        nabla_b.push_back(delta);
        nabla_w.push_back(delta.dot(activations[lastLyr - lyr].transpose()));
    }

    /* Debugging code
    if ((nabla_b.back()[0][0] - nabla_b.back().back()[0]) > 0) {
        std::cout << std::setprecision(17) << "nabla_b: "
                  << nabla_b.back()[0][0] << " " << nabla_b.back().back()[0]
                  << " " << nabla_b.back()[5][0]
                  << '\n';
    }
    */

    // Now finally update the weights and biases for each layer. Note
    // that the previous loop computes nabla_b and nabla_w in reverse
    // order. So here we use revLyr variabe to ease accounting for the
    // reverse order in nabla_w and nabla_b
    for (auto lyr = 0, revLyr = lastLyr - 1; (lyr < lastLyr); lyr++, revLyr--) {
        weights[lyr] = weights[lyr] - (nabla_w[revLyr] * eta);
        biases[lyr]  = biases[lyr]  - (nabla_b[revLyr] * eta);
    }
}

// The stream insertion operator to save/write the neural network data
// to a given file or output stream.
std::ostream& operator<<(std::ostream& os, const NeuralNet& nnet) {
    // First print the layer sizes
    os << nnet.layerSizes << '\n';
    // Next print the biases for each layer.
    for (const auto& bias : nnet.biases) {
        os << bias << '\n';
    }
    // Next print the weights for each layer.
    for (const auto& weight : nnet.weights) {
        os << weight << '\n';
    }
    // Return the output stream as per convention
    return os;
}

// The stream extraction operator to load neural network data from a
// given file or input stream.
std::istream& operator>>(std::istream& is, NeuralNet& nnet) {
    // First load the layer sizes
    is >> nnet.layerSizes;
    const int layerCount = nnet.layerSizes[0].size();
    // Now read the biases for each layer
    Matrix temp;
    for (int i = 0; (i < layerCount); i++) {
        is >> temp;
        nnet.biases.push_back(temp);
    }
    // Now read the weights for each layer
    for (int i = 0; (i < layerCount); i++) {
        is >> temp;
        nnet.weights.push_back(temp);
    }
    // Return the input stream as per convention
    return is;    
}


// The method to classify/recognize a given input.
Matrix
NeuralNet::classify(const Matrix& inputs) const {
    Matrix result = inputs;
    for (size_t lyr = 0; (lyr < weights.size()); lyr++) {
        result = (weights[lyr].dot(result) + biases[lyr]).apply(sigmoid);
    }
    return result;
}

#endif
