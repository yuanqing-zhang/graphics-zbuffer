#include "octree.h"
#include "utils.h"

using namespace Eigen;
using namespace std;


OctreeNode::OctreeNode()
{
	for(int i = 0; i < 8; i++) 
		children[i] = nullptr;
}

OctreeNode::OctreeNode(Vector3f b_min, Vector3f b_max, bool is_leaf)
            :is_leaf(is_leaf), bbox_min(b_min), bbox_max(b_max)
{
	for(int i = 0; i < 8; i++) 
		children[i] = nullptr;
}

