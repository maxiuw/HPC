/**
 * A top-level source that exercises the different features of
 * NeuralNet to recognize handwritten digits.  This implementation is
 * essentially based on the implementation from Michael Nielsen at
 * http://neuralnetworksanddeeplearning.com/
 *
 * Copyright (C) 2021 raodm@miamiOH.edu
 */

#include <string>
#include <fstream>
#include <vector>
#include <random>
#include <iomanip>
#include <iostream>
#include <chrono>
#include "NeuralNet.h"

/**
 * Helper method to load a PGM data file into a 1-D matrix that can be
 * supplied as training data to a NeuralNet.
 *
 * \param[in] path The path from where the PGM file is to be loaded.
 *
 * \return A nx1 matrix with each row of the matrix corresponding to a
 * pixel in the image.
 */
Matrix loadPGM(const std::string& path) {
    std::ifstream file(path);
    if (!file.good()) {
        throw std::runtime_error("Unable to read " + path);
    }
    // First read the header and dimensions
    std::string hdr;
    int width, height;
    Val maxVal, value;
    file >> hdr >> width >> height >> maxVal;
    if (hdr != "P2") {
        throw std::runtime_error("Only P2 PGM format is supported");
    }
    // Create a column matrix to read all of the data and normalize it
    Matrix img(width * height, 1);
    for (int i = 0; (i < width * height); i++) {
        file >> value;
        img[i][0] = value / maxVal;
    }
    return img;
}

/**
 * Helper method to compute the expected output for a given image.
 * The expected output is determined from the last digit in a given
 * file name.  For example, if the path is test-image-6883_0.pgm, this
 * method extracts the last "0" in the file name and uses that as the
 * expected digit.  It This method returns a 10x1 matrix with the
 * entry corresponding to the given digit to be set to 1.
 *
 * \paran[in] path The path to the PGM file from where the actual digit is to be extracted.
 */
Matrix getExpectedDigitOutput(const std::string& path) {
    // Path is of the form .../data/TrainingSet/test-image-6883_0.pgm
    // We need to get to the last "_n" part and use 'n' as the label.
    const auto labelPos = path.rfind('_') + 1;
    // Now we know the index position of the 1-digit label.  Convert
    // the character to integer for convenience.
    const int label = path[labelPos] - '0';
    // Now create the expected matrix with the just the value
    // corresponding to the label set to 1.0
    Matrix expected(10, 1, 0.0);
    expected[label][0] = 1.0;  // Just label should be 1.0
    return expected;
}

/**
 * Helper method to use the first \c count number of files to train a
 * given neural network.
 *
 * \param[in,out] net The neural network to be trainined.
 *
 * \param[in] path The prefix path to the location where the training
 * images are actually stored.
 * 
 * \param[in] fileNames The list of PGM image file names to be used
 * for training.
 *
 * \param[in] count The number of files in this list ot be used.
 */
void train(NeuralNet& net, const std::string& path,
           const std::vector<std::string>& fileNames,
           int count = 1e6) {
    for (const auto& imgName : fileNames) {
        const Matrix img = loadPGM(path + "/" + imgName);
        const Matrix exp = getExpectedDigitOutput(imgName);
        net.learn(img, exp);
        if (count-- <= 0) {
            break;
        }
    }
}

/**
 * The top-level method to train a given neural network used a list of
 * files from a given training set.
 *
 * \param[in,out] net The neural network to be trained.
 *
 * \param[in] path The prefix path to the location where the training
 * images are actually stored.
 *
 * \param[in] limit The number of files to be used to train the network.
 *
 * \param[in] imgListFile The file that contains a list of PGM files
 * to be used.  This method randomly shuffles this list before using
 * \c limit nunber of images for training the supplied \c net.
 */
void train(NeuralNet& net, const std::string& path, const int limit = 1e6,
           const std::string& imgListFile = "TrainingSetList.txt") {
    std::ifstream fileList(imgListFile);
    if (!fileList) {
        throw std::runtime_error("Error reading: " + imgListFile);
    }
    std::vector<std::string> fileNames;
    int count = 0;
    // Load the data from the given image file list.
    for (std::string imgName; std::getline(fileList, imgName) &&
             count < limit; count++) {
        fileNames.push_back(imgName);
    }
    // Randomly shuffle the list of file names so that we use a random
    // subset of PGM files for training.
    std::default_random_engine rg;
    std::shuffle(fileNames.begin(), fileNames.end(),
                 std::default_random_engine());
    // Use the helper method to train 
    train(net, path, fileNames, limit);
}

/**
 * Helper method to get the index of the maximum element in a given
 * list. For example maxElemIndex({1, 3, -1, 2}) returns 1.
 *
 * \param[in] vec The vector whose maximum element index is to be
 * returned by this method. This list cannot be empty.
 *
 * \return The index position of the maximum element.
 */
int maxElemIndex(const std::vector<Val>& vec) {
    return std::max_element(vec.begin(), vec.end()) - vec.begin();
}


/**
 * Helper method to determine how well a given neural network has
 * trained used a list of test images.
 *
 * \param[in] net The network to be used for classification.
 *
 * \param[in] path The prefix path to the location where the training
 * images are actually stored.
 * 
 * \param[in] imgFileList A text file containing the list of
 * image-file-names to be used for assessing the effectiveness of the
 * supplied \c net.
 */
void assess(NeuralNet& net, const std::string& path,
            const std::string& imgFileList = "TestingSetList.txt") {
    std::ifstream fileList2(imgFileList);
    if (!fileList2) {
        throw std::runtime_error("Error reading " + imgFileList);
    }
    // Check how many of the images are correctly classified by the
    // given given neural network.
    auto passCount = 0, totCount = 0;;
    for (std::string imgName; std::getline(fileList2, imgName); totCount++) {
        const Matrix img = loadPGM(path + "/" + imgName);
        const Matrix exp = getExpectedDigitOutput(imgName);
        // Have our network classify the image.
        const Matrix res = net.classify(img);
        assert(res.width() == 1);
        assert(res.height() == 10);
        // Find the maximum index positions in exp results to see if
        // they are the same. If they are it is a good
        // result. Otherwise, it is an error.
        const int expIdx = maxElemIndex(exp.transpose()[0]);
        const int resIdx = maxElemIndex(res.transpose()[0]);
        if (expIdx == resIdx) {
            passCount++;
        }
        // std::cout << imgName.substr(imgName.rfind('/'))
        //           << '\t' << expIdx << '\t' << resIdx << '\n';
    }
    std::cout << "Correct classification: " << passCount << " ["
              << (passCount * 1.f / totCount) << "% ]\n";
}

/**
 * The main method that trains and assess a neural network using a
 * given subset of training images.
 *
 * \param[in] argc The numebr of command-line arguments.  This program
 * requires one path where training & test images are stored. It
 * optionally accepts up to 4 optional command-line arguments.
 *
 * \param[in] argv The actual command-line argument.
 *     1. The path where training and test images are stored.
 *     2. The first argument is assumed to be the number of images to
 *        be used.
 *     3. Number of ephocs to be used for training. Default is 30. 
 *     4. The file containing the list of training images to be
 *        used. By default this parameter is set to
 *        "TrainingSetList.txt".
 *     5. The file containing the list of testing images to be
 *        used. By default this parameter is set to
 *        "TestingSetList.txt".
 */
int main(int argc, char *argv[]) {
    // We definitely need 1 argument for the base-path where image
    // files are stored.
    if (argc < 2) {
        std::cout << "Usage: <ImgPath> [#Train] [#Epocs] [TrainSetList] "
                  << "[TestSetList]\n";
        return 1;
    }
    // Process optional command-line arguments or use default values.
    const int imgCount  = (argc > 2 ? std::stoi(argv[2]) : 5000);
    const int epochs    = (argc > 3 ? std::stoi(argv[3]) : 10);    
    const std::string trainImgs = (argc > 4 ? argv[4] : "TrainingSetList.txt");
    const std::string testImgs  = (argc > 5 ? argv[5] : "TestingSetList.txt");

    // Create the neural netowrk
    NeuralNet net({784, 30, 10});
    // Train it in at most 30 epochs.
    for (int i = 0; (i < epochs); i++) {
        std::cout << "-- Epoch #" << i << " --\n";
        std::cout << "Training with " << imgCount << " images...\n";
        const auto startTime = std::chrono::high_resolution_clock::now();
        train(net, argv[1], imgCount, trainImgs);
        assess(net, argv[1], testImgs);
        const auto endTime = std::chrono::high_resolution_clock::now();
        // Compute the timeelapsed for this epoch
        using namespace std::literals;
        std::cout << "Elapsed time = " << ((endTime - startTime) / 1ms)
                  << " milliseconds.\n";
    }
    return 0;
}
