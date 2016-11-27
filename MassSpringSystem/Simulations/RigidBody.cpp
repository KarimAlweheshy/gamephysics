#include "RigidBody.h"

RigidBody::RigidBody(Vec3 position, Vec3 size, int mass) {
	this->position = position;
	this->size = size;
	this->mass = mass;
}
