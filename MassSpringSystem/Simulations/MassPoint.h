 #pragma once

#include "Simulator.h"

class MassPoint
{
public:
	// Constructors
	MassPoint(Vec3 initialPosition, Vec3 initialVelocity, float mass, bool fixed);

	// Members
	Vec3 _Position;
	Vec3 _Velocity;
	float _Mass;
	bool _IsFixed;

	// Functions
	void integratePositions(float timeStep, int integrationMethod);
	void integrateVelocity(float timeStep, int integrationMethod, Vec3 acceleration);

private:
	// formula template y_1 = y_0 + h*x_0
	Vec3 eulerStep(Vec3 old_Y, Vec3 old_X, float h);
	Vec3 midPointStep(Vec3 old_Y, Vec3 old_X, float h);
};

