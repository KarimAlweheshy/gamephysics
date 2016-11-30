#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Simulator.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Vec3 position, Vec3 size, float mass);
	vector<vector<double>> currentInverseTensor();

	//Attributes
	Vec3 position;
	Vec3 size;
	float mass;
	Quat orientation;
	Vec3 angularVelocity;
	Vec3 linearVelocity;
	Vec3 torque;

private:
	vector<vector<double>> tensor;
	vector<vector<double>> inertiaTensor();
};

#endif
