#ifndef _OCTREE_ZBUFFER_H_
#define _OCTREE_ZBUFFER_H_

#include "../io/obj.h"
#include <Eigen/Dense>

class OctreeZBuffer
{
private:
    const int width, height;
    float *pixels;
    objLoader obj;
    float **depth_buffer;
    
public:
    OctreeZBuffer(int w, int h);
    void render_triangle(Eigen::Vector3f A, 
                         Eigen::Vector3f B, 
                         Eigen::Vector3f C);
    void render();

    float *get_pixel(){return pixels;}
    void set_obj(objLoader o){obj = o;}

    
};

#endif