#ifndef _HIERA_ZBUFFER_H_
#define _HIERA_ZBUFFER_H_

#include "../io/obj.h"
#include "z_pyramid.h"
#include <Eigen/Dense>

class HieraZBuffer
{
private:
    const int width, height;
    float *pixels;
    objLoader obj;
    ZPyramid z_pyramid;
    
public:
    HieraZBuffer(int w, int h);
    bool ztest(float maxz, 
               int minx, 
               int miny, 
               int maxx, 
               int maxy);
    void render_triangle(Eigen::Vector3f A, 
                         Eigen::Vector3f B, 
                         Eigen::Vector3f C);
    void render();

    float *get_pixel(){return pixels;}
    void set_obj(objLoader o){obj = o;}

};

#endif