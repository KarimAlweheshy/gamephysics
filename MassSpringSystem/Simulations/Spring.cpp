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
