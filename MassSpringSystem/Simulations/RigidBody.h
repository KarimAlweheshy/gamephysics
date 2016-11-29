#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Simulator.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Vec3 position, Vec3 size, int mass);
	vector<vector<double>> currentTensor();

	//Attributes
	Vec3 position;
	Vec3 size;
	int mass;
	Quat orientation;
	Vec3 angularVelocity;
	Vec3 linearVelocity;

private:
	vector<vector<double>> tensor;
	vector<vector<double>> inertiaTensor();
};

#endif
