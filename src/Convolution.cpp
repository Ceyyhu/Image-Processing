#include <iostream>

#include "Convolution.h"

// Default constructor
Convolution::Convolution() {
    kernel = nullptr;
    kernelHeight = 0;
    kernelWidth = 0;
    stride = 1;
    padding = false;
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad){
    kernelHeight = kh;
    kernelWidth= kw;
    stride = stride_val;
    padding = pad;

    kernel = new double*[kernelHeight];
    for(int i = 0; i < kernelHeight; i++){
        kernel[i] = new double[kernelWidth];
        for(int j = 0; j < kernelHeight; j++){
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other){
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this; // self-assignment check
    }

    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }

    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
    }

    for (int i = 0; i < kernelHeight; ++i) {
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = other.kernel[i][j];
        }
    }

    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int height = input_image.get_height();
    int width = input_image.get_width();

    int paddedHeight = height + 2;
    int paddedWidth = width + 2;

    int convolvedHeight;
    int convolvedWidth;
    ImageMatrix convolvedImage;

    double** Data;
    double** ImageData = input_image.get_data();

    if(padding){
        Data = new double*[paddedHeight];

        for (int i = 0; i < paddedHeight; ++i) {
            Data[i] = new double[paddedWidth];
            for(int j = 0; j < paddedWidth; j++){
                Data[i][j] = 0;
            }
        }

        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                Data[i+1][j+1] = ImageData[i][j];
            }
        }

        convolvedHeight = ( (height - kernelHeight + 2 * 1) / stride ) + 1 ;
        convolvedWidth = ( (width - kernelWidth + 2 * 1) / stride ) + 1;

        convolvedImage = ImageMatrix(convolvedHeight, convolvedWidth);

    }else {
        Data = ImageData;

        convolvedHeight = ( (height - kernelHeight) / stride ) + 1 ;
        convolvedWidth = ( (width - kernelWidth) / stride ) + 1;

        convolvedImage = ImageMatrix(convolvedHeight, convolvedWidth);
    }

    double ** convolvedData = convolvedImage.get_data();

    int currentX;
    int currentY = 0;

    for(int i = 0; i < convolvedHeight; i ++){
        currentX = 0;
        for(int j = 0; j < convolvedWidth; j ++){
            double total = 0;
            for(int m = 0; m < kernelHeight; m++){
                for(int n = 0; n < kernelWidth; n++){
                    total += Data[currentY+m][currentX+n] * kernel[m][n];
                }
            }
            convolvedData[i][j] = total;
            currentX += stride;
        }
        currentY += stride;
    }

    if(padding){
        if (Data != nullptr) {
            for (int i = 0; i < paddedHeight; ++i) {
                delete[] Data[i];
            }
            delete[] Data;
        }
    }

    return convolvedImage;
}
