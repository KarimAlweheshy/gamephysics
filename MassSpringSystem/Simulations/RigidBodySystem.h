#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

class RigidBodySystem {
public:
	// Construtors
	RigidBodySystem();
	void addRigidBody(Vec3 position, Vec3 size, int mass);
	void setOrientationOf(int i, Quat orientation);
	void setVelocityOf(int i, Vec3 velocity);
	void applyForceOnBody(int i, Vec3 loc, Vec3 force);
	Mat4 objectToWorld(int i);

	//Variables
	vector<RigidBody> rigidBodies;

private:
	vector<vector<double>> inertiaTensor(RigidBody rigidBody);
};

#endif
