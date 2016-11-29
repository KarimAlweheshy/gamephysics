#include "Utility.h"

float Utility::getVectorLength(Vec3 vector)
{
	return (float)sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
}

Vec3 Utility::getNormalizedVector(Vec3 vector)
{
	float length = getVectorLength(vector);
	vector.x /= length;
	vector.y /= length;
	vector.z /= length;

	return vector;
}

Vec3 Utility::reverseVectorY(Vec3 vector)
{
	vector.y *= -1;
	return vector;
}

vector<vector<double>> Utility::dotProduct(vector<vector<double>> firstOperand, vector<vector<double>> secondOperand) {
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

vector<vector<double>> Utility::threeDMatrixFromMat4(Mat4 matrix) {
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
