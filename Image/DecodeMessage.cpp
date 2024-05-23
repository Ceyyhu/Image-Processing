#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string messageBit;
    double** imgData = image.get_data();
    for(std::pair<int,int> myPair: edgePixels){
        int pixel = (int) image.get_data(myPair.first, myPair.second);
        std::string lsb = std::to_string((pixel & 1));
        messageBit += lsb;
    }

    int remainder = (int) (messageBit.length() % 7);

    if(remainder != 0){
        int toAdd = 7 - remainder;
        messageBit.insert(0, toAdd, '0');
    }

    int iterAmount = (int) (messageBit.length() / 7);
    std::string decodedMessage;

    for(int i = 0; i < iterAmount; i++){
        std::string bitString = messageBit.substr(i * 7, 7);
        int pos = 0;
        int power = 1;
        int decimal = 0;
        while(pos < 7){
            int val = std::stoi(std::string(1, bitString[6-pos]));
            if (val == 1){
                decimal += power;
            }
            pos++;
            power *= 2;
        }

        if(decimal <= 32){
            decimal += 33;
        } else if(decimal >= 127){
            decimal = 126;
        }

        char asciiValue = (char) decimal;

        decodedMessage += asciiValue;

    }

    return decodedMessage;
}

