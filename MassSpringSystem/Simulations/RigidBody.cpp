#include "RigidBody.h"
#include "Utility.h"

RigidBody::RigidBody(Vec3 position, Vec3 size, int mass) {
	this->position = position;
	this->size = size;
	this->mass = mass;

	//Calculate inertia tensors
	tensor = inertiaTensor();
}

vector<vector<double>> RigidBody::inertiaTensor() {
	//Init vector
	vector<vector<double>> tensor = vector<vector<double>>();

	//Calculate diagonals
	double i00 = (1.0 / 12.0) * (double)mass * (pow(size.z, 2) + pow(size.y, 2));
	double i11 = (1.0 / 12.0) * (double)mass * (pow(size.x, 2) + pow(size.z, 2));
	double i22 = (1.0 / 12.0) * (double)mass * (pow(size.x, 2) + pow(size.y, 2));

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

vector<vector<double>> RigidBody::currentTensor() {
	Mat4 rotationMatrix = orientation.getRotMat();
	vector<vector<double>> rotationMatrix3D = Utility::threeDMatrixFromMat4(rotationMatrix);

	Mat4 inverseRotationMatrix = rotationMatrix.inverse();
	vector<vector<double>> inverseRotationMatrix3D = Utility::threeDMatrixFromMat4(inverseRotationMatrix);

	vector<vector<double>> result = Utility::dotProduct(Utility::dotProduct(rotationMatrix3D, tensor), inverseRotationMatrix3D);
	return result;
}
