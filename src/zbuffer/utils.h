#ifndef __UTILS_H__
#define __UTILS_H__

#include <Eigen/Dense>

bool is_point_in_triangle(Eigen::Vector2f A, 
                          Eigen::Vector2f B, 
                          Eigen::Vector2f C, 
                          Eigen::Vector2f P);


float cal_depth(Eigen::Vector3f A, 
                Eigen::Vector3f B, 
                Eigen::Vector3f C,
                Eigen::Vector2f P);


float cal_diffuse(Eigen::Vector3f A, 
                  Eigen::Vector3f B, 
                  Eigen::Vector3f C);

#endif