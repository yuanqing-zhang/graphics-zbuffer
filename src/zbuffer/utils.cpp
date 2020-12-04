#include "utils.h"
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

bool is_point_in_triangle(Vector2f A, Vector2f B, Vector2f C, Vector2f P)
{
    // check whether point is in triangle in 2D
    Vector2f v0 = C - A;
    Vector2f v1 = B - A;
    Vector2f v2 = P - A;

    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);

    float inver = 1 / (dot00 * dot11 - dot01 * dot01);

    float u = (dot11 * dot02 - dot01 * dot12) * inver;
    if (u < 0 || u > 1)
        return false;

    float v = (dot00 * dot12 - dot01 * dot02) * inver;
    if (v < 0 || v > 1)
        return false;

    return u + v <= 1;
}


float cal_depth(Vector3f A, 
                Vector3f B, 
                Vector3f C,
                Vector2f P)
{
    Vector3f face_normal = (B - A).cross(C - A);
    face_normal.normalize();

    Vector3f line_normal(0, 0, -1);
    line_normal.normalize();

    Vector3f line_p(P(0), P(1), 0);

    float t;
    t = (face_normal.dot(A) - face_normal.dot(line_p)) 
            / (face_normal.dot(line_normal));

    Vector3f p = line_p + t * line_normal;

    return p(2);
}


float cal_diffuse(Vector3f A, 
                  Vector3f B, 
                  Vector3f C)
{
    Vector3f face_normal = (B - A).cross(C - A);
    face_normal.normalize();

    Vector3f light_dir(0, 0, -1);
    light_dir.normalize();

    float diffuse = - face_normal.dot(light_dir);

    return diffuse;
}