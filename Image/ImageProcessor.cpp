#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {
}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {

    ImageMatrix sharpImage = sharpener.sharpen(img, k);
    std::vector<std::pair<int,int>> EdgePixels = edger.detectEdges(sharpImage);

    DecodeMessage decoder;
    std::string decodedMessage = decoder.decodeFromImage(sharpImage, EdgePixels);

    return decodedMessage;
}


ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {

    ImageMatrix sharpImage = sharpener.sharpen(img, k);
    std::vector<std::pair<int,int>> EdgePixels = edger.detectEdges(sharpImage);

    EncodeMessage encoder;
    return encoder.encodeMessageToImage(img, message, EdgePixels);
}
