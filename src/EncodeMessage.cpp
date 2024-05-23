#include "EncodeMessage.h"
#include <cmath>


// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {

}

int fibonacci(int n){
    if(n == 0){
        return 0;
    }
    if(n <= 2){
        return 1;
    }
    return fibonacci(n-1) + fibonacci(n-2);
}

std::string toBinaryString(char ch){
    int decimal = ((int) ch);
    std::string binaryStr;
    while(decimal >= 1){
        if(decimal % 2 == 1){
            binaryStr.insert(0, 1, '1');
        } else {
            binaryStr.insert(0, 1, '0');
        }
        decimal /= 2;
    }
    int remainder = (int) (7 - binaryStr.length());
    if(binaryStr.length() < 7){
        binaryStr.insert(0, remainder, '0');
    }
    return binaryStr;
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    int remainder = (int) (positions.size() % 7);
    int toTruncate = 7 - remainder;

    std::string newMessage = message.substr(0, 2);
    for(int i = 2; i < message.length(); i++){
        bool prime = true;
        char Ascii = message[i];
        for(int j = 2; j <= i/2; j++){
            if(i % j == 0 ){
                prime = false;
            }
        }
        if(prime){
            int decimal =  ((int) message[i]) + fibonacci(i);
            if(decimal <= 32){
                decimal += 33;
            } else if(decimal >= 127){
                decimal = 126;
            }
            Ascii = (char) decimal;

        }
        newMessage += Ascii;
    }

    int shiftAmount = (int) ( newMessage.length() / 2 );
    std::string firstPart = newMessage.substr(newMessage.length() - shiftAmount, shiftAmount);
    std::string secondPart = newMessage.substr(0, newMessage.length() - shiftAmount);
    newMessage = firstPart + secondPart;

    std::string binaryString;
    for(char i : newMessage){
        binaryString += toBinaryString(i);
    }
    
    if(binaryString.length() > positions.size()){
       binaryString = binaryString.substr(0, binaryString.length() - toTruncate);
    }
    ImageMatrix newImg(img.get_height(),img.get_width());

    for(int i = 0; i < newImg.get_height(); i++){
        for(int j = 0;j < newImg.get_width(); j++){
        newImg.get_data()[i][j] = img.get_data(i,j);
        }
    }

    int pos = 0;
    for(std::pair<int,int>pairs : positions){
        if(pos == binaryString.length()){
            break;
        }

        if(binaryString[pos] == '0'){
            newImg.get_data()[pairs.first][pairs.second] = ( (int) newImg.get_data(pairs.first, pairs.second) ) >> 1;
            newImg.get_data()[pairs.first][pairs.second] = ( (int) newImg.get_data(pairs.first, pairs.second) ) << 1;
        } else {
            newImg.get_data()[pairs.first][pairs.second] = ( (int) newImg.get_data(pairs.first, pairs.second) ) >> 1;
            newImg.get_data()[pairs.first][pairs.second] = ( ( (int) newImg.get_data(pairs.first, pairs.second) ) << 1 )| 1;
        }
        pos++;
    }

    return newImg;
}
