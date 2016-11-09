#include "Spring.h"


Spring::Spring(MassPoint point1, MassPoint point2, float initialLength, float relaxedLength, int stiffness)
{
	_MassPoints.push_back(point1);
	_MassPoints.push_back(point2);
	_CurrentLength = initialLength;
	_RelaxedLength = relaxedLength;
	_Stiffness = stiffness;
}


void Spring::calculateElasticForces()
{

}



