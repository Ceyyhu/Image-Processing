#include "ImageSharpening.h"
// Default constructor
ImageSharpening::ImageSharpening() {
    blurring_kernel = nullptr;
    kernel_height = 0;
    kernel_width = 0;
}

ImageSharpening::~ImageSharpening(){
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernel_height; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    kernel_height = 3;
    kernel_width = 3;
    double multiplier = ((double) 1)/9;

    blurring_kernel = new double*[kernel_height];
    for(int i = 0; i < kernel_height; i++){
        blurring_kernel[i] = new double[kernel_width];
        for(int j = 0; j < kernel_width; j++){
            blurring_kernel[i][j] = multiplier * 1;
        }
    }

    Convolution convObject(blurring_kernel, kernel_height, kernel_width, 1, true);
    ImageMatrix blurredImage = convObject.convolve(input_image);

    ImageMatrix sharpImage = input_image + (input_image - blurredImage) * k;

    int sharpHeight = sharpImage.get_height();
    int sharpWidth = sharpImage.get_width();
    double** sharpData = sharpImage.get_data();

    for(int i = 0; i < sharpHeight; i++){
        for(int j = 0; j < sharpWidth; j++){
            double val = sharpImage.get_data(i,j);
            if(val < 0){
                sharpData[i][j] = 0;
            } else if(val > 255){
                sharpData[i][j] = 255;
            }
        }
    }

    return sharpImage;
}
