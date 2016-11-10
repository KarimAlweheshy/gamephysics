#pragma once

#include "Simulator.h"
#include "MassPoint.h"

class Spring
{
public:
	// Constructor
	Spring(MassPoint point1, MassPoint point2, float initialLength, float stifness);
	
	// Members
	std::vector<MassPoint> _MassPoints;
	float _Stifness;
	float _InitialLength;

	// Functions
	void calculateElasticForces();
	float getCurrentLength();
	Vec3 getInitialLengthVector();
	Vec3 getCurrentLengthVector();
	Vec3 getMassPoint1Acceleration();
	Vec3 getMassPoint2Acceleration();
};
