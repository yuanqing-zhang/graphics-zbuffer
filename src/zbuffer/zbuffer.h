#include "../io/obj.h"
#include "utils.h"
#include <Eigen/Dense>

class ZBuffer
{
private:
    const int width, height;
    float *pixels;
    objLoader obj;
    
public:
    ZBuffer(int w, int h);
    void render();

    float *get_pixel(){return pixels;}
    void set_obj(objLoader o){obj = o; std::cout << obj.v_mat << std::endl;}

};