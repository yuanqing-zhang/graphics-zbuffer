#include "hiera_zbuffer.h"
#include "utils.h"

using namespace Eigen;
using namespace std;


HieraZBuffer::HieraZBuffer(int w, int h) : width(w), height(h)
{
    pixels = new float[3 * width * height];

    depth_buffer = new float*[width];
    // init depth with -infinity
    for(int i = 0; i < width; i++)
    {
        depth_buffer[i] = new float[height]();
        for(int j = 0; j < height; j++)
            depth_buffer[i][j] = - std::numeric_limits<float>::max();
    }
}


// bool ztest(Vector3f A, Vector3f B, Vector3f C)
// {
//     //图像金字塔
//     float minz = min(min(A(2), B(2)), C(2));

// }

// void HieraZBuffer::render_triangle(Vector3f A, Vector3f B, Vector3f C)
// {
//     if(!ztest(A, B, C))
//         return;
//     // find bounding box of triangle
//     float minx = min(min(A(0), B(0)), C(0));
//     float maxx = max(max(A(0), B(0)), C(0));

//     float miny = min(min(A(1), B(1)), C(1));
//     float maxy = max(max(A(1), B(1)), C(1));

//     if(minx >= height || miny >= width) return;
//     if(maxx <=0 || maxy <= 0) return;

//     // 2D projection of A,B,C
//     Vector2f A_2d, B_2d, C_2d;
//     A_2d = A.head(2);
//     B_2d = B.head(2);
//     C_2d = C.head(2);
    
//     for(int i = miny; i < maxy; i++)
//     {
//         for(int j = minx; j < maxx; j++)
//         {
//             Vector2f P(j, i);
//             if(is_point_in_triangle(A_2d, B_2d, C_2d, P))
//             {

//                 float depth = cal_depth(A, B, C, P);
//                 if(depth_buffer[j][i] < depth)
//                 {
//                     depth_buffer[j][i] = depth;
//                     float color = (depth + width / 2) / width;
//                     // float color = cal_diffuse(A, B, C);
//                     pixels[3 * i * width + 3 * j + 0] = color;
//                     pixels[3 * i * width + 3 * j + 1] = color;
//                     pixels[3 * i * width + 3 * j + 2] = color;
//                 }
//             }

//             update_z_hiera();
//         }
//     }
// }


// void HieraZBuffer::render() 
// {
//     for(int f_idx = 0; f_idx < obj.f_num; f_idx++)
//     {
//         // word corordinate up->x, right->y, outward->z
//         Vector3f A, B, C;
//         A << obj.v_mat.block(0, obj.f_set[f_idx][0], 3, 1);
//         B << obj.v_mat.block(0, obj.f_set[f_idx][1], 3, 1);
//         C << obj.v_mat.block(0, obj.f_set[f_idx][2], 3, 1); 
 
//         //scale and translate to the center of image
//         Vector3f center(width / 2, width / 2, 0);
//         A = A * width / 2 + center;
//         B = B * width / 2 + center;
//         C = C * width / 2 + center;

//         // render triangle depth and update depth buffer
//         render_triangle(A, B, C);

//         // handle quadrangle
//         if(obj.f_set[0].size() > 3)
//         {
//             Vector3f A, B, C;
//             A << obj.v_mat.block(0, obj.f_set[f_idx][0], 3, 1);
//             B << obj.v_mat.block(0, obj.f_set[f_idx][2], 3, 1);
//             C << obj.v_mat.block(0, obj.f_set[f_idx][3], 3, 1); 
    
//             //scale and translate to the center of image
//             Vector3f center(width / 2, width / 2, 0);
//             A = A * width / 2 + center;
//             B = B * width / 2 + center;
//             C = C * width / 2 + center;

//             // render triangle depth and update depth buffer
//             render_triangle(A, B, C);

//         }
//     }
// }