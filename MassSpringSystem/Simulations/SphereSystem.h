#pragma once
#include "Simulator.h"
#include "Sphere.h"
#include "config.h"

class SphereSystem
{
public:
	SphereSystem();

	vector<Sphere> spheres;
	Vec3 externalForce;
	collisionType type;
	Vec3 color;
};

