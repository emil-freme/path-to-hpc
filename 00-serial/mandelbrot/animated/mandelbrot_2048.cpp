#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

typedef struct complexNumber{
    double x;
    double i;
} complexNumber;

typedef struct domain{
    double min_x;
    double max_x;
    double min_y;
    double max_y;
} sdomain;


uint8_t map_to_byte(unsigned int o_max, unsigned int value){
   return (uint8_t)((value * 255) / o_max);
}

double map_max(double o_max, double t_max, double value){
    return (value * t_max) / o_max;
}

double map_min_max(double o_min, double o_max, double t_min, double t_max, double value){
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

double lerp(double a, double b, double t){
    return a + (b-a)*t;
}

int main(void){

    const char* magicIdentifyer = "P2";
    const int width = 256;
    const int maxIter = 100;


    //Animation parameters
    const int start_zoom = 1;
    const int end_zoom = 10;
    const complexNumber start_center = {0, 0};//{(-2.75,1)(-1,1)}
    const complexNumber end_center = {-0.7091, -0.02844};
    const int steps = 500;

    //Set calculation
    int actual_step = 0;

animation:
    uint8_t rawImage[width][width];

    complexNumber center;
    center.x = lerp(start_center.x, end_center.x, (double)actual_step/(double)steps);
    center.i = lerp(start_center.i, end_center.i, (double)actual_step/(double)steps);

    sdomain domain;
    double zoom = lerp(start_zoom, end_zoom, (double)actual_step/(double)steps);
    domain.min_x = (center.x - 2/zoom);
    domain.max_x = (center.x + 2/zoom);
    domain.min_y = (center.x - 2/zoom);
    domain.max_y = (center.x + 2/zoom);

    for(int col = 0; col < width; col++){
        for(int row = 0; row < width; row++){

            double startingX = map_min_max(0, width, domain.min_x, domain.max_x, row);
            double startingI = map_min_max(0, width, domain.min_y, domain.max_y, col);

            complexNumber startingPoint = {startingX, startingI};

            int calculated = mandelbrot(startingPoint, center, 0, maxIter);

            rawImage[col][row] = map_to_byte(maxIter, calculated);
        }
    }

    //Writing to file
    ostringstream filename;
    filename << "mandelbrot_" << actual_step << ".pgm";
    ofstream file(filename.str(), ios::out | ios::trunc);
    if(file.is_open()){
        file << magicIdentifyer <<"\n";
        file << width << " " << width << "\n";
        file << 255 << "\n";

        for(int col = 0; col < width; col++){
            for(int row = 0; row < width; row++){
                file << (int)rawImage[col][row] << " ";
            }
            file << "\n";
        }

        file.close();
    }

    if (actual_step < steps){
        actual_step++;
        goto animation;
    }



    return 0;
}
