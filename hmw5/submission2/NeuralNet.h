#ifndef NEURAL_NET_H
#define NEURAL_NET_H

/**
 * A simple neural network implementationi n C++.  This implementation
 * is essentially based on the implementation from Michael Nielsen at
 * http://neuralnetworksanddeeplearning.com/
 *
 * Copyright (C) 2021 raodm@miamiOH.edu
 */

#include <iostream>
#include <functional>
#include <vector>
#include <tuple>
#include <string>
#include <cstdlib>
#include "Matrix.h"

// A vector containing a list of doubles
using DoubleVec = std::vector<double>;

// A list of matrixes that is used to represent the biases and weights
// associated with each layer of the neural net.
using MatrixVec = std::vector<Matrix>;

/**
 * The main NeuralNetwork class. This class is sufficiently flexible
 * to enable creating different neural networks with different number
 * of layers and sizes.
 */
class NeuralNet {
    /**
     * A stream insertion operator to save/write the neural network so
     * that the trained network can be saved and loaded easily.
     *
     * \param[out] os The output stream to where the data is to be
     * written.
     *
     * \param[in] nnet The neural network to be serialized to the
     * given output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const NeuralNet& nnet);

    /**
     * The stream extraction operator to read data for a neural
     * network based on data written earlier via the above stream
     * insertion operator.
     *
     * \param[in] is The intput stream from where the data is to be
     * read.
     *
     * \param[out] nnet The neural network whose data is to be
     * read/modified by this method.
     */
    friend std::istream& operator>>(std::istream& is, NeuralNet& nnet);    

public:
    /**
     * Creates a neural network with a given number of layers with a
     * given number of neurons at each layer. For example NeuralNet
     * net({784, 30, 10}); creates a \c nnet with 3-layers with the
     * input layer having 784 neurons, hidden layer with 30 neurons,
     * and 10 neurons in the output layer.
     *
     * \param[in] layers The layers and number of neurons on each
     * layer.x
     */
    NeuralNet(const std::vector<int>& layers);

    /**
     * The helper method that updates the weights and biases of the
     * network to help it recognize the given input image as a digit.
     *
     * \param[in] inputs The input pixels that contain the image to be
     * recognized by the neural network.  The number of pixels in this
     * image must be exactly the same as the number of input neurons
     * for this neural network.
     *
     * \param[in] expected The expected output matrix for this image.
     * This matrix should be the same dimension as the output layer of
     * this neural network.
     *
     * \param[in] eta The learning rate at which this neural network
     * is to learn from this one example.
     */
    void learn(const Matrix& inputs, const Matrix& expected,
               const Val eta = 0.3);

    /**
     * This method is used to classify or recognize a given image
     * based on the current learning by this neural network.
     *
     * \param[in] inputs The input image to be classified/recognized
     * by this neural network.  The number of pixels in this image
     * must be exactly the same as the number of input neurons for
     * this neural network.
     *
     * \return The output matrix resulting from
     * classifying/recognizing the input image.
     */
    Matrix classify(const Matrix& inputs) const;

    /**
     * This method is the top-level training method that processes
     * multiple input images and calling the learn method in this
     * class.
     *
     * \param[in] path Path to a file that contains the list of input
     * images to be used by this method to train the neural network.
     */
    void train(const std::string& path);

protected:
    /**
     * This is an internal helper method that is used to initializes
     * the biases and weights matrix values for each layer.  This
     * method is used in the constructor to initialize the set of
     * matrices used by this class.
     *
     * \param[in] layerSizes The number of layers and number of
     * neurons in each layer.
     *
     * \param[out] biases The list of biases for each layer to be
     * initialized by this method.
     *
     * \param[out] weights The list of weights for each layer to be
     * initialized by this method.
     */
    void initBiasAndWeightMatrices(const std::vector<int>& layerSizes,
                                   MatrixVec& biases, MatrixVec& weights) const;

    /**
     * A simple sigmoid function.
     *
     * \param[in] val The value whose sigmoid value is to be returned.
     *
     * \return The sigmoid value for the given val.
     */
    static Val sigmoid(const Val val) {
        return 1. / (1. + std::exp(-val));
    }

    
    /**
     * A simple inverse-sigmoid function.
     *
     * \param[in] val The value whose inverse sigmoid value is to be
     * returned.
     *
     * \return The inverse sigmoid value for the given val.
     */
    static Val invSigmoid(const Val val) {
        return sigmoid(val) * (1 - sigmoid(val));
    }

private:
    /**
     * The column-vector of biases associated with each layer of the
     * neural network.
     */
    MatrixVec biases;

    /**
     * The two dimensional matrix of weights associated with each
     * layer of the neural network.
     */
    MatrixVec weights;

    /**
     * The number of neurons to be present on each layer of the neural
     * network.
     */
    Matrix layerSizes;
};

#endif
