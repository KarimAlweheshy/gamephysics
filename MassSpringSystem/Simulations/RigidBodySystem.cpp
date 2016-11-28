#include "RigidBodySystem.h"
#include "Simulator.h"

RigidBodySystem::RigidBodySystem() {
	rigidBodies = vector<RigidBody>();
}

void RigidBodySystem::addRigidBody(Vec3 position, Vec3 size, int mass) {
	RigidBody rigidBody = RigidBody(position, size, mass);
	rigidBodies.push_back(rigidBody);
}

void RigidBodySystem::setOrientationOf(int i, Quat orientation) {
	rigidBodies[i].orientation = orientation;
}

void RigidBodySystem::setVelocityOf(int i, Vec3 velocity) {
	rigidBodies[i].linearVelocity = velocity;
}
void RigidBodySystem::applyForceOnBody(int i, Vec3 loc, Vec3 force) {
	//What is loc
}

Mat4 RigidBodySystem::objectToWorld(int i) {
	RigidBody rigidBody = rigidBodies[i];

	Mat4 translationMatrix = Mat4(rigidBody.size.x, 0, 0, 0,
								  0, rigidBody.size.y, 0, 0, 
								  0, 0, rigidBody.size.z, 0, 
								  0, 0, 0,				  1);

	Mat4 rotationMatrix = rigidBody.orientation.getRotMat();

	return translationMatrix * rotationMatrix;
}
