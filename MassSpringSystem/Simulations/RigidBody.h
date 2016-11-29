#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Simulator.h"

class RigidBody {
public:
	//Constructor
	RigidBody(Vec3 position, Vec3 size, int mass);
	vector<vector<double>> currentTensor();

	//Attributes
	Vec3 position;
	Vec3 size;
	int mass;
	Quat orientation;
	Vec3 angularVelocity;
	Vec3 linearVelocity;

private:
	vector<vector<double>> tensor;
	vector<vector<double>> inertiaTensor();
	vector<vector<double>> threeDMatrixFromMat4(Mat4 matrix);
	vector<vector<double>> dotMultiplication(vector<vector<double>> firstOperand, vector<vector<double>> secondOperand);
};

#endif