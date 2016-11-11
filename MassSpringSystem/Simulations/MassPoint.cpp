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
		case Simulator::Integrator::Euler: 
			_Position = eulerStep(_Position, _Velocity, timeStep);
			//cout << _Position;
			break;
		case::Simulator::Integrator::MidPoint:
			_Position = eulerStep(_Position, _Velocity, timeStep);
			break;
		}
	}

}

void MassPoint::integrateVelocity(float timeStep, int integrationMethod, Vec3 acceleration)
{
	// No movement if point is fixed
	if (!_IsFixed)
	{
		switch (integrationMethod)
		{
			case Simulator::Integrator::Euler: 
				_Velocity = eulerStep(_Velocity, acceleration, timeStep);
				//cout << _Velocity;
				break;
			case::Simulator::Integrator::MidPoint: 
				_Velocity = eulerStep(_Velocity, acceleration, timeStep); 
				break;
		}
	}
}

Vec3 MassPoint::eulerStep(Vec3 old_Y, Vec3 old_X, float h)
{
	// example newvelo = oldvelo + time*old_acceler
	return old_Y + h*old_X;
}
