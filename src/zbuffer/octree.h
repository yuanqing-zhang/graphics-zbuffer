#pragma once

#include <vector>
#include <Eigen/Dense>

class OctreeNode
{
public:
    Eigen::Vector3f bbox_min;  // bounding box of node
    Eigen::Vector3f bbox_max;
    /* 
    pointers to child octants
    child:	0 1 2 3 4 5 6 7
        x:  0 0 0 0 1 1 1 1
        y:  0 0 1 1 0 0 1 1
        z:  0 1 0 1 0 1 0 1 
    */
    OctreeNode *children[8];   
    std::vector<std::vector<int>> f_set;  // faces in this node
    bool is_leaf;
    

    OctreeNode();
    OctreeNode(Eigen::Vector3f b_min, 
               Eigen::Vector3f b_max, 
               bool is_leaf);

};
