#pragma once

#include "Simulator.h"
#include "MassPoint.h"

class Spring
{
public:
	// Constructor
	Spring(MassPoint point1, MassPoint point2, float initialLength, float relaxedLength, int stiffness);
	
	// Members
	std::vector<MassPoint> _MassPoints;
	float _CurrentLength;

	// Functions
	void calculateElasticForces();
	
private:
	float _RelaxedLength;
	int _Stiffness;

};

