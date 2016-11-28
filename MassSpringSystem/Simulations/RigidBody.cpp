#include "RigidBody.h"

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

	//Calculate diagonal item
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
	vector<vector<double>> rotationMatrix3D = threeDMatrixFromMat4(rotationMatrix);

	Mat4 inverseRotationMatrix = rotationMatrix.inverse();
	vector<vector<double>> inverseRotationMatrix3D = threeDMatrixFromMat4(inverseRotationMatrix);

	vector<vector<double>> result = dotMultiplication(dotMultiplication(rotationMatrix3D, tensor), inverseRotationMatrix3D);
	return result;
}

vector<vector<double>> RigidBody::threeDMatrixFromMat4(Mat4 matrix) {
	vector<vector<double>> result = vector<vector<double>>();

	vector<double> firstRow = vector<double>();
	firstRow.push_back(matrix.value[0][0]);
	firstRow.push_back(matrix.value[0][1]);
	firstRow.push_back(matrix.value[0][2]);
	result.push_back(firstRow);

	vector<double> secondRow = vector<double>();
	secondRow.push_back(matrix.value[1][0]);
	secondRow.push_back(matrix.value[1][1]);
	secondRow.push_back(matrix.value[1][2]);
	result.push_back(secondRow);

	vector<double> thirdRow = vector<double>();
	thirdRow.push_back(matrix.value[2][0]);
	thirdRow.push_back(matrix.value[2][1]);
	thirdRow.push_back(matrix.value[2][2]);
	result.push_back(thirdRow);

	return result;
}

vector<vector<double>> RigidBody::dotMultiplication(vector<vector<double>> firstOperand, vector<vector<double>> secondOperand) {
	vector<vector<double>> resultMatrix = firstOperand;
	for (uint16_t i = 0; i < firstOperand.size(); i++) {
		for (uint16_t j = 0; j < firstOperand.size(); j++) {
			//calculate for i row j column
			double result = 0;
			for (uint16_t k = 0; k < firstOperand.size(); k++) {
				result += firstOperand[i][k] * secondOperand[k][j];
			}
			resultMatrix[i][j] = result;
		}
	}
	return resultMatrix;
}
