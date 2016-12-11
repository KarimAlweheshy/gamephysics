#ifndef UTILITY_H
#define UTILITY_H

#include "Simulator.h"


class Utility {
public:
	static float getVectorLength(Vec3 vector);
	static Vec3 getNormalizedVector(Vec3 vector);
	static Vec3 reverseVectorY(Vec3 vector);
	static vector<vector<float>> dotProduct(vector<vector<float>> firstOperand, vector<vector<float>> secondOperand);
	static float scalarProduct(Vec3 a, Vec3 b);
	static vector<vector<float>> threeDMatrixFromMat4(Mat4 matrix);
	static Vec3 MatrixVectorProduct3D(vector<Vec3> matrix, Vec3 vector);
};

#endif
