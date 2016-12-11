#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Simulator.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Vec3 position, Vec3 size, float mass);
	vector<vector<float>> currentInverseTensor();
	vector<Vec3> getTensorInWorldspace();
	Mat4 getObjectToWorld();

	//Attributes
	Vec3 position;
	Vec3 size;
	float mass;
	Quat orientation;
	Vec3 angularVelocity;
	Vec3 linearVelocity;
	Vec3 torque;


private:
	vector<Vec3> tensor;
	vector<vector<float>> inertiaTensor();

	void calculateTensor();
};

#endif
