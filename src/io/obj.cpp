#include "obj.h"

#include <fstream>
#include <sstream>

using namespace std;
using namespace Eigen;

#define REPORT_LOCATION __FILE__ << " " << __LINE__ << ": "


inline bool is_line_valid(const string &line) {
	return (line.empty() || line[0] == 13 || line[0] == '#');
}


inline void report_error(const string &info) {
	cerr << "ERROR: in " << REPORT_LOCATION << info << endl;
	assert(0);
}


void objLoader::initMat(fstream &f)
{
	string line;
	string type;

	v_num = 0;
	f_num = 0;
	vn_num = 0;

	while (getline(f, line)) 
	{
		if (is_line_valid(line)) continue;
		istringstream instream(line);
		instream >> type;
		if (type == "v" || type == "V")
			v_num++;
		else if (type == "f" || type == "F") 
			f_num++;
		else if (type == "vn" || type == "VN") 
			vn_num++;
		type.clear();
	}
	f.clear();
	f.seekg(0, ios::beg);

	if (v_num == 0 || f_num == 0)
		report_error("no vertex or face data found!");

	v_mat.resize(4, v_num);
	vn_mat.resize(3, vn_num);
	f_set.resize(f_num);
	fn_set.resize(f_num);

}


void objLoader::load_obj(string filename)
{
	string line;
	string type;
	fstream f;
	size_t v_idx = 0, f_idx = 0, vn_idx = 0;

	// open obj file and check 
	f.open(filename, ios::in);
	if (!f.is_open())
	{
		report_error("fail to open obj file");
		return;
	}

	cout << ">>> start reading obj file in " << filename << endl;
	initMat(f);

	// read line by line
	while (getline(f, line))
	{
		if (is_line_valid(line)) continue;
		istringstream instream(line);
		instream >> type;
		if (type == "v" || type == "V") 
		{
			instream >> v_mat(0, v_idx) 
					 >> v_mat(1, v_idx) 
					 >> v_mat(2, v_idx);

			// read w and recomput vertex cordinate
			double w = 1.0;
			if (instream >> w) 
			{
				if (w < 1e-6) 
					report_error("error occured when read vertex w");
				for (size_t i = 0; i < 3; i++)
					v_mat(i, v_idx) /= w;
			}
			v_mat(3, v_idx) = 1;
			v_idx++;
		}
		else if (type == "vn" || type == "VN") 
		{
			instream >> vn_mat(0, vn_idx) 
					 >> vn_mat(1, vn_idx) 
					 >> vn_mat(2, vn_idx);
			vn_idx++;
		}
		else if (type == "f" || type == "F")
		{
			while(true)
			{
				string sub_group;
				instream >> sub_group;
				if(sub_group.empty()) break;

				// get vertex index
				string vertex_str = sub_group.substr(0, sub_group.find('/'));
				long unsigned int v_index;
				sscanf(vertex_str.c_str(), "%lu", &v_index);
				f_set[f_idx].push_back(v_index - 1); // obj file index start from 1
				// cout << v_index << "/";

				// get vertex normal index if exist
				string vn_str = sub_group.substr(sub_group.rfind('/') + 1, sub_group.size());
				if (vn_str.size() == 0) continue;
				long unsigned int vn_index;
				sscanf(vn_str.c_str(), "%lu", &vn_index);
				fn_set[f_idx].push_back(vn_index - 1);
				// cout << vn_index << " ";
			}
			// cout << endl;
			f_idx++;
		}
		type.clear();
	}

	f.close();

	MatrixXd::Index max_row, max_col;
	float max = v_mat.maxCoeff(&max_row,&max_col);
	v_mat = v_mat / max;

	// handle quadrangle
	if(f_set[0].size() > 3)
	{
		f_set.resize(f_num * 2);
		for(int i = 0; i < f_num; i++)
		{
			f_set[f_num + i].push_back(f_set[i][0]);
			f_set[f_num + i].push_back(f_set[i][2]);
			f_set[f_num + i].push_back(f_set[i][3]);
		}
		f_num = f_num * 2;
	}
}


void objLoader::rotate(Quaternionf q)
{
	q = q.normalized();

	for(int i = 0; i < v_num; i++)
	{
		Vector3f v, rot_v, vn, rot_vn;
        v << v_mat.block(0, i, 3, 1);

		rot_v = q * v;
		v_mat.block(0, i, 3, 1) = rot_v.head(3);

		// TODO: normal rotation
		// vn << vn_mat.block(0, i, 3, 1);
		// rot_vn = q.inverse().normalized() * vn;
		// vn_mat.block(0, i, 3, 1) = rot_vn.head(3);
		// cout << vn_mat.block(0, i, 3, 1) << endl;
	}
}