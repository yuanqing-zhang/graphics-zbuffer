#include <GLUT/glut.h> 
#include <string>

#include "io/obj.h"
#include "view/View.h"
#include "zbuffer/zbuffer.h"
#include "zbuffer/basic_zbuffer.h"
#include "zbuffer/hiera_zbuffer.h"
#include "zbuffer/octree_zbuffer.h"

using namespace std;
using namespace Eigen;


int width = 512;
int height = 512;
View view;
string type;
objLoader obj;


void display(void) 
{ 
    // test green background 
    // glClearColor(0.0f, 1.0f, 0.0f, 1.0f); 

    glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();

    if(type == "1")
    {
        ZBuffer zbuff(width, height);
        zbuff.set_obj(obj);
	    zbuff.render();
	    glDrawPixels(width, height, GL_RGB, GL_FLOAT, zbuff.get_pixel());
    }
    else if(type == "2")
    {
        BasicZBuffer basic_zbuff(width, height);
        basic_zbuff.set_obj(obj);
	    basic_zbuff.render();
	    glDrawPixels(width, height, GL_RGB, GL_FLOAT, basic_zbuff.get_pixel());
    }
    else if(type == "3")
    {
        BasicZBuffer hiera_zbuff(width, height);
        hiera_zbuff.set_obj(obj);
	    hiera_zbuff.render();
	    glDrawPixels(width, height, GL_RGB, GL_FLOAT, hiera_zbuff.get_pixel());
    }
    // else
    // {
    //     OctreeZBuffer octree_zbuff(width, height);
    //     octree_zbuff.set_obj(obj);
	//     octree_zbuff.render();
	//     glDrawPixels(width, height, GL_RGB, GL_FLOAT, octree_zbuff.get_pixel());
    // }

    glFlush(); 
}


int main(int argc, char *argv[]) 
{ 
	if (argc < 2 || argc > 3) {
		cerr << "Usage: render <type> <model_path>" << endl;
		cerr << "type can be 1:zbuffer 2:zbuffer(basic) "
             << "3:hierarchical zbuffer(basic) 4:hierarchical zbuffer(octree) " << endl;
		return 1;
	}
	type = argv[1];
    
    string filepath = "../src/models/cube.obj";
	if (argc == 3) filepath = argv[2];

    //load obj file and rotate
	obj.load_obj(filepath);
    Quaternionf q(cos((M_PI / 2) / 2), 
				  0 * sin((M_PI / 2) / 2), 
				  0 * sin((M_PI / 2) / 2), 
				  1 * sin((M_PI / 2) / 2));
    // obj.rotate(q);

    glutInit(&argc, argv); 

    // setting up the display RGB color model
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH); 

    // configure window position and size 
    glutInitWindowPosition(100, 100); 
    glutInitWindowSize(width, height); 

    // create window 
    glutCreateWindow("Graphics"); 

    // call to the drawing function 
    glutDisplayFunc(display); 
    // keyboard control
    // glutKeyboardFunc(keyboard);

    // loop require by opengl 
    glutMainLoop(); 
    return 0; 
} 