#include "Sphere.h"


Sphere::Sphere(Vec3 initialPosition, Vec3 initialVelocity)
{
	_Position = initialPosition;
	_Velocity = initialVelocity;
}

void Sphere::integratePositions(float timeStep)
{
	_Position = eulerStep(_Position, _Velocity, timeStep);
}

void Sphere::integrateVelocity(float timeStep, Vec3 acceleration)
{
	_Velocity = eulerStep(_Velocity, acceleration, timeStep);
}

Vec3 Sphere::eulerStep(Vec3 old_Y, Vec3 old_X, float h)
{
	// example newvelo = oldvelo + time*old_acceler
	return old_Y + h * old_X;
}