# ifndef _Z_PYRAMID_H_
# define _Z_PYRAMID_H_

# include "utils.h"

class ZPyramid
{
private:
    int width;  // width of the finest level
    int height; // height of the finest level

public:
    int level;  // level of the Z pyramid
    float*** zvalue; // depth buffer pyramid
    ZPyramid();
    ZPyramid(int w, int h);

    void update(int x, int y, float value);
};

# endif
