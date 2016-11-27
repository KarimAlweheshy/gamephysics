#include "RigidBodySystem.h"
#include "Simulator.h"

RigidBodySystem::RigidBodySystem() {
	rigidBodies = vector<RigidBody>();
}

void RigidBodySystem::addRigidBody(Vec3 position, Vec3 size, int mass) {
	RigidBody rigidBody = RigidBody(position, size, mass);

	//Calculate inertia tensors
	rigidBody.tensor = inertiaTensor(rigidBody);

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
	//(translatMat * rotMat * scaleMat)
	return Mat4();
}

vector<vector<double>> RigidBodySystem::inertiaTensor(RigidBody rigidBody) {
	//Init vector
	vector<vector<double>> tensor = vector<vector<double>>();

	//Calculate diagonal item
	double i00 = 1 / 12 * rigidBody.mass * (pow(rigidBody.size.z, 2) + pow(rigidBody.size.y, 2));
	double i11 = 1 / 12 * rigidBody.mass * (pow(rigidBody.size.x, 2) + pow(rigidBody.size.z, 2));
	double i22 = 1 / 12 * rigidBody.mass * (pow(rigidBody.size.x, 2) + pow(rigidBody.size.y, 2));

	vector<double> xVector = vector<double>();
	xVector[0] = i00;

	vector<double> yVector = vector<double>();
	yVector[0] = i11;

	vector<double> zVector = vector<double>();
	zVector[0] = i22;

	tensor.push_back(xVector);
	tensor.push_back(yVector);
	tensor.push_back(zVector);

	return tensor;
}
