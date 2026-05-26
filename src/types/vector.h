#ifndef VECTOR_H
#define VECTOR_H

struct Vector4 {
	double x;
	double y;
	double z;
	double w;

	Vector4():
		x(0), y(0), z(0), w(0)
	{}

	Vector4(double nx, double ny, double nz, double nw):
		x(nx), y(ny), z(nz), w(nw)
	{}
};

#endif