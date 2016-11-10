#include "Spring.h"

Spring::Spring(MassPoint point1, MassPoint point2, float initialLength, float stifness)
{
	_MassPoints.push_back(point1);
	_MassPoints.push_back(point2);
	_InitialLength = initialLength;
	_Stifness = stifness;
}

void Spring::calculateElasticForces()
{

}

float Spring::getCurrentLength() {
	Vec3 point1 = _MassPoints[0]._Position;
	Vec3 point2 = _MassPoints[1]._Position;
	return (float) hypot(hypot(point1.X - point1.X, point1.Y - point2.Y), point1.Z - point2.Z);
}

Vec3 Spring::getInitialLengthVector() {
	return _InitialLength * (getCurrentLengthVector() / getCurrentLength());
}

Vec3 Spring::getCurrentLengthVector() {
	return _MassPoints[1]._Position - _MassPoints[0]._Position;
}

Vec3 Spring::getMassPoint1Acceleration() {
	return -_Stifness * (getCurrentLengthVector() - getInitialLengthVector()) / _MassPoints[0]._Mass;
}

Vec3 Spring::getMassPoint2Acceleration() {
	return -_Stifness * (getCurrentLengthVector() - getInitialLengthVector()) / _MassPoints[1]._Mass;
}