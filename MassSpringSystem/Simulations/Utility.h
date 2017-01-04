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
	static float getVectorDistance(Vec3 vec1, Vec3 vec2);

	//http://stackoverflow.com/a/5289624
	static float getRandomFloat(float a, float b);
};

#endif
