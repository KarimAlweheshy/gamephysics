#include "MassPoint.h"


MassPoint::MassPoint(Vec3 initialPosition, Vec3 initialVelocity, float mass, bool fixed)
{
	_Position = initialPosition;
	_Velocity = initialVelocity;
	_Mass = mass;
	_IsFixed = fixed;
}

void MassPoint::integratePositions(float timeStep, int integrationMethod)
{
	// No movement if point is fixed
	if (!_IsFixed)
	{
		switch (integrationMethod)
		{
		case Simulator::Integrator::Euler: break;
		case::Simulator::Integrator::MidPoint:
			cout << _Position.x;
			_Position = _Position + _Velocity*timeStep;
			break;
		}
	}

}

void MassPoint::integrateVelocity(float timeStep, int integrationMethod)
{
	// No movement if point is fixed
	if (!_IsFixed)
	{
		switch (integrationMethod)
		{
			case Simulator::Integrator::Euler: break;
			case::Simulator::Integrator::MidPoint: break;
		}
	}
}




