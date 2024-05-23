// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    vertical_kernel = nullptr;
    horizontal_kernel = nullptr;
    magnitudeArray = nullptr;

    verticalHeight = 0;
    verticalWidth = 0;
    horizontalHeight = 0;
    horizontalWidth = 0;

    imageHeight = 0;
    imageWidth = 0;
}

// Destructor
EdgeDetector::~EdgeDetector() {
    if (vertical_kernel != nullptr) {
        for (int i = 0; i < verticalHeight; ++i) {
            delete[] vertical_kernel[i];
        }
        delete[] vertical_kernel;
    }
    if (horizontal_kernel != nullptr) {
        for (int i = 0; i < horizontalHeight; ++i) {
            delete[] horizontal_kernel[i];
        }
        delete[] horizontal_kernel;
    }
    if (magnitudeArray != nullptr) {
        for (int i = 0; i < imageHeight; ++i) {
            delete[] magnitudeArray[i];
        }
        delete[] magnitudeArray;
    }
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    double sobelVertical[3][3] = {
            {-1,0,1},
            {-2,0,2},
            {-1,0,1},
    };
    double sobelHorizontal[3][3] = {
            {-1,-2,-1},
            {0,0,0},
            {1,2,1},
    };

    verticalHeight = 3;
    verticalWidth = 3;

    horizontalHeight = 3;
    horizontalWidth = 3;

    vertical_kernel = new double*[verticalHeight];
    for(int i = 0; i < verticalHeight; i++){
        vertical_kernel[i] = new double[verticalWidth];
        for(int j = 0; j < verticalWidth; j++){
            vertical_kernel[i][j] = sobelVertical[i][j];
        }
    }

    horizontal_kernel = new double*[horizontalHeight];
    for(int i = 0; i < horizontalHeight; i++){
        horizontal_kernel[i] = new double[horizontalWidth];
        for(int j = 0; j < horizontalWidth; j++){
            horizontal_kernel[i][j] = sobelHorizontal[i][j];
        }
    }

    Convolution verticalConvolution(vertical_kernel, 3, 3, 1, true);
    Convolution horizontalConvolution(horizontal_kernel, 3, 3, 1, true);

    ImageMatrix imageVertical = verticalConvolution.convolve(input_image);
    ImageMatrix imageHorizontal = horizontalConvolution.convolve(input_image);
    // matrix dimensions are preserved
    imageHeight = input_image.get_height();
    imageWidth = input_image.get_width();

    magnitudeArray = new double*[imageHeight];
    for(int i = 0; i < imageHeight; i++){
        magnitudeArray[i] = new double[imageWidth];
    }

    double total = 0;
    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            double val = sqrt(pow(imageVertical.get_data(i,j), 2) + pow(imageHorizontal.get_data(i,j), 2));
            magnitudeArray[i][j] = val;
            total += val;
        }
    }



    double threshold = total / (imageHeight * imageWidth);

    std::vector<std::pair<int, int>> EdgePixels;

    for(int i = 0; i < imageHeight; i++){
        for(int j = 0; j < imageWidth; j++){
            if(magnitudeArray[i][j] > threshold){
                EdgePixels.push_back(std::make_pair(i,j));

            }
        }
    }

    return EdgePixels;
}

