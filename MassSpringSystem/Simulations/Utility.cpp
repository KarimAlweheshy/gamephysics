#include "Utility.h"

float Utility::getVectorLength(Vec3 vector)
{
	return sqrt(pow(vector.x, 2) + pow(vector.y, 2) + pow(vector.z, 2));
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


