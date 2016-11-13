#include "MassPoint.h"


MassPoint::MassPoint(Vec3 initialPosition, Vec3 initialVelocity, float mass, bool fixed)
{
	_Position = initialPosition;
	_Velocity = initialVelocity;
	_Mass = mass;
	_IsFixed = fixed;
}

void MassPoint::integratePositions(float timeStep)
{
	// No movement if point is fixed
	if (!_IsFixed)
	{
		_Position = eulerStep(_Position, _Velocity, timeStep);
	}

}

void MassPoint::integrateVelocity(float timeStep, Vec3 acceleration)
{
	// No movement if point is fixed
	if (!_IsFixed)
	{
		_Velocity = eulerStep(_Velocity, acceleration, timeStep);
	}
}

Vec3 MassPoint::eulerStep(Vec3 old_Y, Vec3 old_X, float h)
{
	// example newvelo = oldvelo + time*old_acceler
	return old_Y + h * old_X;
}
