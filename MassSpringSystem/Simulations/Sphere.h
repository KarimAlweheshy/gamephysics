#pragma once

#include "Simulator.h"

class Sphere
{
public:
	// Constructors
	Sphere(Vec3 initialPosition, Vec3 initialVelocity);

	// Members
	Vec3 _Position;
	Vec3 _Velocity;

	// Functions
	void integratePositions(float timeStep);
	void integrateVelocity(float timeStep, Vec3 acceleration);

private:
	// formula template y_1 = y_0 + h*x_0
	Vec3 eulerStep(Vec3 old_Y, Vec3 old_X, float h);
};

