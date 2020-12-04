#pragma once

#include <Eigen/Dense>

#define PI 3.1415926

class View {
private:
	void update() {
		eye[0] = radius * cos(phi) * sin(alpha);
		eye[1] = radius * sin(phi);
		eye[2] = radius * cos(phi) * cos(alpha);
	}
public:
	Eigen::Vector3f eye, center, up;
	float radius, alpha, phi;
	View() {
		radius = 1, alpha = 0, phi = 0;
		center << 0, 0, 0;
		up << 0, 1, 0;
		update();
	}
	void set_radius(float r) {
		radius = r;
		update();
	}
	void set_alpha(float a) {
		alpha = a;
		update();
	}
	void set_phi(float p) {
		phi = p;
		if (phi > PI / 2) {
			phi = PI / 2;
		}
		if (phi < -PI / 2) {
			phi = -PI / 2;
		}
		update();
	}
};