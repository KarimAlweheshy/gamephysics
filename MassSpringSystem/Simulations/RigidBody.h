#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Simulator.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Vec3 position, Vec3 size, int mass);

	//Attributes
	Vec3 position;
	Vec3 size;
	int mass;
	Quat orientation;
	Vec3 angularVelocity;
	Vec3 linearVelocity;
	vector<vector<double>> tensor;
};

#endif
