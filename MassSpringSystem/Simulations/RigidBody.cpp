#include "RigidBody.h"
#include "Utility.h"
#include "math.h"

RigidBody::RigidBody(Vec3 position, Vec3 size, float mass) {
	this->position = position;
	this->size = size;
	this->mass = mass;

	this->orientation = Quat();
	this->angularVelocity = Vec3();
	this->linearVelocity = Vec3();
	this->torque = Vec3();

	//Calculate inertia tensors
	tensor = inertiaTensor();
}

vector<vector<double>> RigidBody::inertiaTensor() {
	//Init vector
	vector<vector<double>> tensor = vector<vector<double>>();

	//Calculate diagonals
	double i00 = (1.0 / 12.0) * mass * (pow(size.z, 2) + pow(size.y, 2));
	double i11 = (1.0 / 12.0) * mass * (pow(size.x, 2) + pow(size.z, 2));
	double i22 = (1.0 / 12.0) * mass * (pow(size.x, 2) + pow(size.y, 2));

	vector<double> xVector = vector<double>(3);
	xVector[0] = i00;

	vector<double> yVector = vector<double>(3);
	yVector[1] = i11;

	vector<double> zVector = vector<double>(3);
	zVector[2] = i22;

	tensor.push_back(xVector);
	tensor.push_back(yVector);
	tensor.push_back(zVector);

	return tensor;
}

vector<vector<double>> RigidBody::currentInverseTensor() {
	Mat4 rotationMatrix = orientation.getRotMat().inverse();
	vector<vector<double>> rotationMatrix3D = Utility::threeDMatrixFromMat4(rotationMatrix);

	rotationMatrix.transpose();
	vector<vector<double>> inverseRotationMatrix3D = Utility::threeDMatrixFromMat4(rotationMatrix);

	vector<vector<double>> result = Utility::dotProduct(Utility::dotProduct(rotationMatrix3D, tensor), inverseRotationMatrix3D);
	return result;
}
