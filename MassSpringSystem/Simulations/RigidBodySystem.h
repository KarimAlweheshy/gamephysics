#ifndef RIGIDBODYSYSTEM_H
#define RIGIDBODYSYSTEM_H

#include "RigidBody.h"

struct ForceOnRigidBody {
	Vec3 loc;
	Vec3 force;
	int onRigidBodyIndex;
};

class RigidBodySystem {
public:
	// Construtors
	RigidBodySystem();
	void addRigidBody(Vec3 position, Vec3 size, int mass);
	void setOrientationOf(int i, Quat orientation);
	void setVelocityOf(int i, Vec3 velocity);
	void applyForceOnBody(int i, Vec3 loc, Vec3 force);
	void resetForceSOnAllBodies();
	vector<Vec3> forcesVectorsOnRigidBodyWithIndex(int index);
	Mat4 objectToWorld(int i);

	//Variables
	vector<RigidBody> rigidBodies;

private:
	vector<ForceOnRigidBody> forcesOnRigidBodies;
};

#endif
