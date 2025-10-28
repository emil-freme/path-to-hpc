#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

uint8_t map_to_byte(unsigned int o_max, unsigned int value){
   return (uint8_t)((value * 255) / o_max);
}

int main(void){

    const char* magicIdentifyer = "P2";
    const int width = 4098;
    const int grayDepth = 255;

    vector<uint8_t> rawImage(width * width);

    for(int col = 0; col < width; col++){
        for(int row = 0; row < width; row++){
            int index = row * width + col;
            rawImage[index] = map_to_byte(width, (row + col)/2);
        }
    }

    ofstream file("gradient.pgm", ios::out | ios::trunc);
    if(file.is_open()){
        file << magicIdentifyer <<"\n";
        file << width << " " << width << "\n";
        file << grayDepth << "\n";

        for(int col = 0; col < width; col++){
            for(int row = 0; row < width; row++){
                int index = row * width + col;
                file << (int)rawImage[index] << " ";
            }
            file << "\n";
        }

        file.close();
    }

    return 0;
}
