#include "zbuffer.h"

using namespace Eigen;
using namespace std;

ZBuffer::ZBuffer(int w, int h) : width(w), height(h)
{
    pixels = new float[3 * width * height];
}

void ZBuffer::render()
{
    float min_value = - std::numeric_limits<float>::max();
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            float max_z = min_value;
            float color = 0;
            for(int f_idx = 0; f_idx < obj.f_num; f_idx++)
            {
                Vector3f A, B, C;
                A << obj.v_mat.block(0, obj.f_set[f_idx][0], 3, 1);
                B << obj.v_mat.block(0, obj.f_set[f_idx][1], 3, 1);
                C << obj.v_mat.block(0, obj.f_set[f_idx][2], 3, 1); 
                
                //scale and translate to the center of image
                Vector3f center(width / 2, width / 2, 0);
                A = A * width / 2 + center;
                B = B * width / 2 + center;
                C = C * width / 2 + center;

                Vector2f A_2d, B_2d, C_2d;
                A_2d = A.head(2);
                B_2d = B.head(2);
                C_2d = C.head(2);

                Vector2f P(j, i);
                if(is_point_in_triangle(A_2d, B_2d, C_2d, P))
                {
                    float _z = cal_depth(A, B, C, P); // depth
                    if(_z > max_z)
                    {
                        max_z = _z;
                        color = cal_diffuse(A, B, C);
                    }
                }
            }

            if(max_z != min_value || max_z != -min_value) // not null 
            {
                color = (max_z + width / 2) / width * 0.5;
                pixels[3 * j * width + 3 * i + 0] = color;
                pixels[3 * j * width + 3 * i + 1] = color;
                pixels[3 * j * width + 3 * i + 2] = color;
            }
        }
    }
}