#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <string>
#include "geometry.h"

#define PI 3.14159265358979323846

void render() {
    const int mapWidth    = 1024;
    const int mapHeight   = 768;

    std::vector<Vec3f> framebuffer(mapWidth*mapHeight);

    #pragma omp parallel for
    for (size_t j = 0; j<mapHeight; j++) {
        for (size_t i = 0; i<mapWidth; i++) {
            float r =  255;
            float g = 255;
            float b = 255;
            framebuffer[i+j*mapWidth] = Vec3f(r,g,b);
        }
    }

    std::ifstream inFile("airports.txt");

    for (int counter = 0; !inFile.eof(); counter++)
    {
        float latitude    = 0; // (φ)
        float longitude   = 0;   // (λ)

        std::string temp = "";
        getline(inFile,temp);
        latitude = strtof((temp).c_str(),0);
        getline(inFile,temp);
        longitude = strtof((temp).c_str(),0);

        float x = (longitude+180)*(mapWidth/360);
        float latRad = latitude*PI/180;
        float mercN = log(tan((PI/4)+(latRad/2)));
        float y = (mapHeight/2)-(mapWidth*mercN/(2*PI));

        std::cout << latitude << " : " << longitude << std::endl;
        std::cout << x << " : " << y << std::endl;

        framebuffer[x+y*mapWidth] = Vec3f(255,0,0);
    }

    std::ofstream ofs; // save the framebuffer to file
    ofs.open("./out.ppm");
    ofs << "P6\n" << mapWidth << " " << mapHeight << "\n255\n";
    for (size_t i = 0; i < mapHeight*mapWidth; ++i) {
        Vec3f &c = framebuffer[i];
        float max = std::max(c[0], std::max(c[1], c[2]));
        if (max>1) c = c*(1./max);
        for (size_t j = 0; j<3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}

int main() {

    render();

    return 0;
}
