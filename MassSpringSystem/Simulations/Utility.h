#pragma once

#include "Simulator.h"


class Utility {
public:
	static float getVectorLength(Vec3 vector);
	static Vec3 getNormalizedVector(Vec3 vector);
	static Vec3 reverseVectorY(Vec3 vector);
	static vector<vector<double>> dotProduct(vector<vector<double>> firstOperand, vector<vector<double>> secondOperand);
	static vector<vector<double>> threeDMatrixFromMat4(Mat4 matrix);
};

