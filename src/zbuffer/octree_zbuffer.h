#pragma once

#include "../io/obj.h"
#include "octree.h"
#include "z_pyramid.h"
#include <Eigen/Dense>

class OctreeZBuffer
{
private:
    const int width, height;
    float *pixels;
    objLoader obj;
    ZPyramid z_pyramid;
    
public:
    OctreeNode *octree_root;

    OctreeZBuffer(int w, int h);
    bool ztest(float maxz, 
               int minx, 
               int miny, 
               int maxx, 
               int maxy);
    void render_triangle(Eigen::Vector3f A, 
                         Eigen::Vector3f B, 
                         Eigen::Vector3f C);
    void recurr_render(OctreeNode *octree_node, int level);

    float *get_pixel(){return pixels;}
    void set_obj(objLoader o);
    
};
