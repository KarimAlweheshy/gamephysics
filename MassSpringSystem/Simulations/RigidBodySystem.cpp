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
	Mat4 translationMatrix = Mat4(rigidBody.size.x, 0, 0, 0, 0, rigidBody.size.y, 0, 0, 0, 0, rigidBody.size.z, 0, 0, 0, 0, 1);

	Mat4 rotationMatrix = rigidBody.orientation.getRotMat();
	
	//Identity scale matrix
	Mat4 identityMatrix = Mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	
	// The identity matrix holds for the scale Matrix
	Mat4 objectToWorld = translationMatrix * rotationMatrix * identityMatrix;

	return objectToWorld;
}

vector<vector<double>> RigidBodySystem::inertiaTensor(RigidBody rigidBody) {
	//Init vector
	vector<vector<double>> tensor = vector<vector<double>>();

	//Calculate diagonal item
	double i00 = 1 / 12 * rigidBody.mass * (pow(rigidBody.size.z, 2) + pow(rigidBody.size.y, 2));
	double i11 = 1 / 12 * rigidBody.mass * (pow(rigidBody.size.x, 2) + pow(rigidBody.size.z, 2));
	double i22 = 1 / 12 * rigidBody.mass * (pow(rigidBody.size.x, 2) + pow(rigidBody.size.y, 2));

	vector<double> xVector = vector<double>(3);
	xVector[0] = i00;

	vector<double> yVector = vector<double>(3);
	yVector[0] = i11;

	vector<double> zVector = vector<double>(3);
	zVector[0] = i22;

	tensor.push_back(xVector);
	tensor.push_back(yVector);
	tensor.push_back(zVector);

	return tensor;
}
