#pragma once

#include <Eigen/Dense>
#include <set>
#include <vector>
#include <iostream>

// obj format:
//List of geometric vertices
// v x, y, z [,w]
// List of texture coordinates
// vt u, v [,w]
// List of vertex normals
// vn x, y, z
// Polygonal face element(normal/texture is optimal)
// f v1[/vt1/vn1] v2[/vt2/vn2] v3[/vt3/vn3]


class objLoader{
public:
	objLoader(){};

	void initMat(std::fstream &f);
	void load_obj(std::string filename);
	void rotate(Eigen::Quaternionf q);

	size_t v_num, f_num, vn_num;

	Eigen::MatrixXf v_mat;  // vertices
	Eigen::MatrixXf vn_mat;  // vertex normals
	std::vector<std::vector<int>> f_set;  // index of vertices
	std::vector<std::vector<int>> fn_set; // index of normals
};