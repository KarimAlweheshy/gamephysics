#include "RigidBodySystem.h"
#include "Simulator.h"

RigidBodySystem::RigidBodySystem() {
	rigidBodies = vector<RigidBody>();
	forcesOnRigidBodies = vector<ForceOnRigidBody>();
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
	ForceOnRigidBody forceOnRigidBody = ForceOnRigidBody();
	forceOnRigidBody.force = force;
	forceOnRigidBody.loc = loc;
	forceOnRigidBody.onRigidBodyIndex = i;
	forcesOnRigidBodies.push_back(forceOnRigidBody);
}

void RigidBodySystem::resetForceSOnAllBodies() {
	forcesOnRigidBodies = vector<ForceOnRigidBody>();
}

vector<ForceOnRigidBody> RigidBodySystem::forcesOnRigidBodyWithIndex(int index) {
	vector<ForceOnRigidBody> forcesVector = vector<ForceOnRigidBody>();
	for (uint16_t i = 0; i < forcesOnRigidBodies.size(); i++) {
		ForceOnRigidBody currentForce = forcesOnRigidBodies[i];
		if (currentForce.onRigidBodyIndex == index) {
			forcesVector.push_back(currentForce);
		}
	}
	return forcesVector;
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
