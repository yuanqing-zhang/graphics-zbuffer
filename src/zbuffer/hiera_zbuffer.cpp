#include "hiera_zbuffer.h"
#include "utils.h"

using namespace Eigen;
using namespace std;


HieraZBuffer::HieraZBuffer(int w, int h) : width(w), height(h)
{
    pixels = new float[3 * width * height];
    // construct z-pyramid
    z_pyramid = ZPyramid(width, height);

}


bool HieraZBuffer::ztest(float minz, int minx, int miny, int maxx, int maxy)
{
    // find the finest-level sample of the pyramid whose 
    // corresponding image region covers the bounding box of the polygon
    int cover_level = 0;

    while(true)
    {
        if(minx == maxx && miny == maxy)
            break;
        minx = minx / 2;
        miny = miny / 2;
        maxx = maxx / 2;
        maxy = maxy / 2;
        cover_level += 1;
    }
    int p_maxz = z_pyramid.zvalue[cover_level][minx][miny];

    if(p_maxz > minz)
        return false;
    else
        return true;

}

void HieraZBuffer::render_triangle(Vector3f A, Vector3f B, Vector3f C)
{    
    //scale and translate to the center of image
    Vector3f center(width / 2, width / 2, 0);
    A = A * width / 2 + center;
    B = B * width / 2 + center;
    C = C * width / 2 + center;

    // find bounding box of triangle
    float minx = min(min(A(0), B(0)), C(0)) - 1;
    float maxx = max(max(A(0), B(0)), C(0)) - 1;

    float miny = min(min(A(1), B(1)), C(1)) - 1;
    float maxy = max(max(A(1), B(1)), C(1)) - 1;

    // if triangle out of viewing frustum, then return
    if(minx >= (height - 3) || miny >= (width - 3)) return;
    if(maxx <= 3 || maxy <= 3) return;

    // test whether it is hidden
    float minz = min(min(A(2), B(2)), C(2));
    if(!ztest(minz, minx, maxx, miny, maxy))
        return;

    // 2D projection of A,B,C
    Vector2f A_2d, B_2d, C_2d;
    A_2d = A.head(2);
    B_2d = B.head(2);
    C_2d = C.head(2);
    
    for(int i = miny; i < maxy; i++)
    {
        for(int j = minx; j < maxx; j++)
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


void HieraZBuffer::render() 
{
    for(int f_idx = 0; f_idx < obj.f_num; f_idx++)
    {
        // word corordinate up->x, right->y, outward->z
        Vector3f A, B, C;
        A << obj.v_mat.block(0, obj.f_set[f_idx][0], 3, 1);
        B << obj.v_mat.block(0, obj.f_set[f_idx][1], 3, 1);
        C << obj.v_mat.block(0, obj.f_set[f_idx][2], 3, 1); 

        // render triangle depth and update depth buffer
        render_triangle(A, B, C);

        // handle quadrangle
        if(obj.f_set[0].size() > 3)
        {
            Vector3f A, B, C;
            A << obj.v_mat.block(0, obj.f_set[f_idx][0], 3, 1);
            B << obj.v_mat.block(0, obj.f_set[f_idx][2], 3, 1);
            C << obj.v_mat.block(0, obj.f_set[f_idx][3], 3, 1); 

            // render triangle depth and update depth buffer
            render_triangle(A, B, C);
        }
    }
}