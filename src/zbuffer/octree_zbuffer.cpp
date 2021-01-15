#include "octree_zbuffer.h"
#include "utils.h"

using namespace Eigen;
using namespace std;


OctreeZBuffer::OctreeZBuffer(int w, int h) : width(w), height(h)
{
    pixels = new float[3 * width * height];
    // construct z-pyramid
    z_pyramid = ZPyramid(width, height);
}


OctreeNode *build(Vector3f b_min, 
                  Vector3f b_max, 
                  MatrixXf v_mat,
                  vector<vector<int>> face_set)
{
    if(face_set.size() <= 10)
    {
        OctreeNode *leaf_node = new OctreeNode(b_min, b_max, true);
        for(int i = 0; i < face_set.size(); i++)
        {
            leaf_node->f_set.push_back(face_set[i]);
        }
        return leaf_node;
    }

    //divede bounding box into 8 sub-bounding box
    OctreeNode *curr_node = new OctreeNode(b_min, b_max, false);
    vector<Vector3f> sub_b_min = get_8_b_min(b_min, b_max);
    vector<Vector3f> sub_b_max = get_8_b_max(b_min, b_max);

    //divede face_set into 8 subset
    vector<vector<int>> sub_face_set[8];
    for(int f_idx = 0; f_idx < face_set.size(); f_idx++)
    {
        Vector3f A, B, C;
        A << v_mat.block(0, face_set[f_idx][0], 3, 1);
        B << v_mat.block(0, face_set[f_idx][1], 3, 1);
        C << v_mat.block(0, face_set[f_idx][2], 3, 1); 
        bool is_f_contain = false;
        for (int i = 0; i < 8; i++)
        {
            if(is_contain(sub_b_min[i], sub_b_max[i], A, B, C))
            {
                sub_face_set[i].push_back(face_set[f_idx]);
                is_f_contain = true;
            }
        }
        // if no sub-node contain, put in the current node
        if(is_f_contain == false)
            curr_node->f_set.push_back(face_set[f_idx]);
    }
    // recursive insertion
    for(int i = 0; i < 8; i++) 
        curr_node->children[i] = build(sub_b_min[i], 
                                       sub_b_max[i],
                                       v_mat,
                                       sub_face_set[i]);
    return curr_node;
}


void OctreeZBuffer::set_obj(objLoader o)
{
    obj = o;
    //scale and translate all veterx to the center of image
    Vector3f center(width / 2, width / 2, 0);
    for(int v_idx = 0; v_idx < obj.v_num; v_idx++)
    {
        Vector3f v;
        v << obj.v_mat.block(0, v_idx, 3, 1);
        v = (v - obj.center) * width * 0.95 + center;
        obj.v_mat.block(0, v_idx, 3, 1) = v;
    }

    // get root cube
    Vector3f bbox_min;
    Vector3f bbox_max;

    bbox_min << obj.v_mat.block(0, 0, 3, 1);
    bbox_max << obj.v_mat.block(0, 0, 3, 1);

    for(int v_idx = 1; v_idx < obj.v_num; v_idx++)
    {
        Vector3f v;
        v << obj.v_mat.block(0, v_idx, 3, 1);
        bbox_min(0) = min(bbox_min(0), v(0));
        bbox_min(1) = min(bbox_min(1), v(1));
        bbox_min(2) = min(bbox_min(2), v(2));
        bbox_max(0) = max(bbox_max(0), v(0));
        bbox_max(1) = max(bbox_max(1), v(1));
        bbox_max(2) = max(bbox_max(2), v(2));
    }
    // placing the geometry into an octree
    octree_root = build(bbox_min * 0.8, bbox_max * 1.25, obj.v_mat, obj.f_set);
    cout << ">>> finish build octree." << endl;
}


bool OctreeZBuffer::ztest(float maxz, int minx, int miny, int maxx, int maxy)
{
    // find the finest-level sample of the pyramid whose 
    // corresponding image region covers the bounding box of the polygon
    int cover_level = 0;
    int x = maxx - minx;
    int y = maxy - miny;

    while(true)
    {
        if(x >= y && (minx == maxx))
            break;
        if(x < y && (miny == maxy))
            break;
        minx = minx / 2;
        miny = miny / 2;
        maxx = maxx / 2;
        maxy = maxy / 2;
        cover_level += 1;

        if(cover_level == z_pyramid.level - 1)
            break;

    }
    int p_minz = z_pyramid.zvalue[cover_level][minx][miny];

    if(p_minz > maxz)
        return false;
    else
        return true;

}


void OctreeZBuffer::render_triangle(Vector3f A, Vector3f B, Vector3f C)
{
    // find bounding box of triangle
    float minx = min(min(A(0), B(0)), C(0));
    float maxx = max(max(A(0), B(0)), C(0));

    float miny = min(min(A(1), B(1)), C(1));
    float maxy = max(max(A(1), B(1)), C(1));

    // if triangle out of viewing frustum, then return    
    if(minx >= height || miny >= width) return;
    if(maxx <=0 || maxy <= 0) return;

    // 2D projection of A,B,C
    Vector2f A_2d, B_2d, C_2d;
    A_2d = A.head(2);
    B_2d = B.head(2);
    C_2d = C.head(2);
    
    for(int i = miny; i <= maxy; i++)
    {
        for(int j = minx; j <= maxx; j++)
        {
            Vector2f P(j, i);
            if(is_point_in_triangle(A_2d, B_2d, C_2d, P))
            {

                float depth = cal_depth(A, B, C, P);
                if(z_pyramid.zvalue[0][j][i] < depth)
                {
                    // maintain the Z pyramid
                    z_pyramid.update(j, i, depth);
                    // float color = (depth + width / 2) / width;
                    float color = cal_diffuse(A, B, C);
                    pixels[3 * i * width + 3 * j + 0] = color;
                    pixels[3 * i * width + 3 * j + 1] = color;
                    pixels[3 * i * width + 3 * j + 2] = color;
                }
            }
        }
    }
}


void OctreeZBuffer::recurr_render(OctreeNode *octree_node, int level) 
{
    // cout << "level " << level << ":" << octree_node->f_set.size() << endl;
    float minx = octree_node->bbox_min(0);
    float miny = octree_node->bbox_min(1);
    float maxx = octree_node->bbox_max(0);
    float maxy = octree_node->bbox_max(1);

    // if node out of viewing frustum, return
    if(minx >= height || miny >= width) return;
    if(maxx <= 0 || maxy <= 0) return;

    // test whether current node is hidden
    if(!ztest(octree_node->bbox_max(2), minx, miny, maxx + 1, maxy + 1))
        return;

    // render current node
    for(int f_idx = 0; f_idx < octree_node->f_set.size(); f_idx++)
    {
        Vector3f A, B, C;
        A << obj.v_mat.block(0, octree_node->f_set[f_idx][0], 3, 1);
        B << obj.v_mat.block(0, octree_node->f_set[f_idx][1], 3, 1);
        C << obj.v_mat.block(0, octree_node->f_set[f_idx][2], 3, 1); 

        // render triangle depth and update depth buffer
        render_triangle(A, B, C);
    }

    if(octree_node->is_leaf)
    {
        return;
    }
    else
    {
        // render sub-node from back to front
        for(int i = 7; i >= 0; i--)
        {
            recurr_render(octree_node->children[i], level + 1);
        }
    }
}