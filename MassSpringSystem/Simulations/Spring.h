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

	// Functions
	void calculateElasticForces();
	float getCurrentLength();
	
private:
	float _InitialLength;
};
