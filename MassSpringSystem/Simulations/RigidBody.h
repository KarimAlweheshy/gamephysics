#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Simulator.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Vec3 position, Vec3 size, float mass);
	vector<vector<float>> currentInverseTensor();

	//Attributes
	Vec3 position;
	Vec3 size;
	float mass;
	Quat orientation;
	Vec3 angularVelocity;
	Vec3 linearVelocity;
	Vec3 torque;

private:
	vector<vector<float>> tensor;
	vector<vector<float>> inertiaTensor();
};

#endif
