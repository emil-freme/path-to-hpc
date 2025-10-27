#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

typedef struct complexNumber{
    float x;
    float i;
} complexNumber;

uint8_t map_to_byte(unsigned int o_max, unsigned int value){
   return (uint8_t)((value * 255) / o_max);
}

float map_max(float o_max, float t_max, float value){
    return (value * t_max) / o_max;
}

float map_min_max(float o_min, float o_max, float t_min, float t_max, float value){
    return ((value - o_min) * (t_max - t_min) / (o_max - o_min)) + t_min;
}

int mandelbrot(complexNumber center, complexNumber point,
        int iter, int maxIter){

    if( (point.x * point.x + point.i * point.i) < 4 && iter < maxIter ){

        complexNumber newPoint;

        newPoint.x = point.x * point.x - point.i * point.i + center.x;
        newPoint.i = 2 * point.x * point.i + center.i;

        return 1 + mandelbrot(center, newPoint, iter+1, maxIter);
    }

    return 0;

}

int main(void){

    const char* magicIdentifyer = "P2";
    const int width = 4096;
    const int maxIter = 500;

    vector<uint8_t> rawImage(width * width);

    complexNumber center = {-0.745, 0.13};

    for(int col = 0; col < width; col++){
        for(int row = 0; row < width; row++){

            float startingX = map_min_max(0, width, -0.7555, -0.74, row);
            float startingI = map_min_max(0, width, 0.11, 0.15, col);

            complexNumber startingPoint = {startingX, startingI};

            int calculated = mandelbrot(startingPoint, center, 0, maxIter);

            int index = row * width + col;

            rawImage[index] = map_to_byte(maxIter, calculated);
        }
    }

    ofstream file("gradient.pgm", ios::out | ios::trunc);
    if(file.is_open()){
        file << magicIdentifyer <<"\n";
        file << width << " " << width << "\n";
        file << 255 << "\n";

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
