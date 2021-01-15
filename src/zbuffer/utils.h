#pragma once

#include<vector>
#include <Eigen/Dense>

using namespace Eigen;

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


int get_level(int width, int height);


std::vector<Eigen::Vector3f> get_8_b_min(Eigen::Vector3f b_min, 
                                         Eigen::Vector3f b_max);


std::vector<Eigen::Vector3f> get_8_b_max(Eigen::Vector3f b_min, 
                                         Eigen::Vector3f b_max);


bool is_point_in_cube(Eigen::Vector3f b_min, 
                      Eigen::Vector3f b_max,
                      Eigen::Vector3f P);


bool is_contain(Eigen::Vector3f b_min, 
                Eigen::Vector3f b_max, 
                Eigen::Vector3f A,
                Eigen::Vector3f B,
                Eigen::Vector3f C);
