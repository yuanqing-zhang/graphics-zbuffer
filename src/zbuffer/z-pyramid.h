# ifndef _Z_PYRAMID_H_
# define _Z_PYRAMID_H_

# include "utils.h"

using namespace std;

class ZPyramid
{
private:
    int width;  // width of the finest level
    int height; // height of the finest level
    int level;  // level of the Z pyramid

public:
    
    float*** zvalue;
    ZPyramid();
    ZPyramid(int w, int h);

    void update(int x, int y, float value);
};

ZPyramid::ZPyramid()
{
    width = 0;
    height = 0;
    level = 0;
}

ZPyramid::ZPyramid(int w, int h) : width(w), height(h)
{
    level = get_level(width, height);

    // init Z pyramid with -infinity
    zvalue = new float**[level];
    for(int l = 0; l < level; l++)
    {
        // init depth with -infinity
        zvalue[l] = new float*[width];
        for(int i = 0; i < width; i++)
        {
            zvalue[l][i] = new float[height];
            for(int j = 0; j < height; j++)
                zvalue[l][i][j] = -std::numeric_limits<float>::max();
        }
    }
}

void ZPyramid::update(int x, int y, float value)
{
    zvalue[0][x][y] = value;

    for(int l = 1; l < level; l++)
    {
        int idx = x / pow(2, l);
        int idy = y / pow(2, l);


        zvalue[l][idx][idy] = min(min(min(zvalue[l - 1][idx * 2][idy * 2],
                                          zvalue[l - 1][idx * 2][idy * 2 + 1]),
                                          zvalue[l - 1][idx * 2 + 1][idy * 2]),
                                          zvalue[l - 1][idx * 2 + 1][idy * 2 + 1]);
        
        if(zvalue[l][idx][idy] == -std::numeric_limits<float>::max())
            break;
    }
}

# endif