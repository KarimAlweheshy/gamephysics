﻿#include "RigidBodySystemSimulator.h"
#include "Utility.h"
#include "collisionDetect.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator() {
	m_pRigidBodySystem = new RigidBodySystem();
}

// Functions
const char * RigidBodySystemSimulator::getTestCasesStr() {
	return "";
}

void RigidBodySystemSimulator::initUI(DrawingUtilitiesClass * DUC) {
	this->DUC = DUC;
}

void RigidBodySystemSimulator::reset() {
	notifyCaseChanged(1);
}

void RigidBodySystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext) {
	// Draw Frame according to calculated new positions for the rigid bodies
	std::mt19937 eng;
	std::uniform_real_distribution<float> randCol(0.0f, 1.0f);
	DUC->setUpLighting(Vec3(), 0.4 * Vec3(1, 1, 1), 100, 0.6 * Vec3(randCol(eng), randCol(eng), randCol(eng)));

	for (uint16_t i = 0; i < m_pRigidBodySystem->rigidBodies.size(); i++) {
		// draw rigid body
		DUC->drawRigidBody(m_pRigidBodySystem->objectToWorld(i));
	}
}

void RigidBodySystemSimulator::notifyCaseChanged(int testCase) {
	if (testCase == 1) {
		m_pRigidBodySystem = new RigidBodySystem();
		
		addRigidBody(Vec3(0.f, 0.f, 0.f), Vec3(1.f, 0.6f, 0.5f), 2.0f);
		setOrientationOf(0, Quat(Vec3(0.0f, 0.0f, 1.0f), (float)(M_PI)*0.5f));

		addRigidBody(Vec3(2.f, 3.f, 0.0f), Vec3(0.7f, 0.4f, 0.3f), 2.0f);
		//setVelocityOf(1, Vec3(0.0f, -0.1f, 0.05f));

		applyForceOnBody(0, Vec3(0.3f, 0.5f, 0.25f), Vec3(1, 1, 0));
		//applyForceOnBody(1, Vec3(0, -0.5f, 0), Vec3(1, 1, 0));
	}
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed) {

}

void RigidBodySystemSimulator::simulateTimestep(float timeStep) {
	for (uint16_t i = 0; i < m_pRigidBodySystem->rigidBodies.size(); i++) {
		//1. Calculate all forces on body
		vector<ForceOnRigidBody> forcesOnBody = m_pRigidBodySystem->forcesOnRigidBodyWithIndex(i);
		Vec3 sumOfForcesVectors = Vec3(0.f);
		for (uint16_t j = 0; j < forcesOnBody.size(); j++) {
			sumOfForcesVectors += forcesOnBody[j].force;
		}

		// 2. Calculate q
		RigidBody currentRigidBody = m_pRigidBodySystem->rigidBodies[i];
		Vec3 q = Vec3(0.f);
		for (uint16_t i = 0; i < forcesOnBody.size(); i++) {
			Vec3 currentX = forcesOnBody[i].loc - currentRigidBody.position;
			Vec3 currentForce = forcesOnBody[i].force;
			q -= cross(currentX, currentForce);
		}

		// 3. Euler Step for center of mass position and velosity
		currentRigidBody.position += (timeStep * currentRigidBody.linearVelocity);
		currentRigidBody.linearVelocity += timeStep * sumOfForcesVectors / currentRigidBody.mass;


		// 4. Update quaternion
		Quat angularVelocity = Quat();
		angularVelocity.x = currentRigidBody.angularVelocity.x * currentRigidBody.orientation.x;
		angularVelocity.y = currentRigidBody.angularVelocity.y * currentRigidBody.orientation.y;
		angularVelocity.z = currentRigidBody.angularVelocity.z * currentRigidBody.orientation.z;
		currentRigidBody.orientation += timeStep / 2.f * angularVelocity;

		// 5. Calculate L
		currentRigidBody.torque = currentRigidBody.torque + (timeStep * q);

		// 6. Get inverse current inertia tensor
		/*
		vector<vector<float>> currentTensor = currentRigidBody.currentInverseTensor();
		vector<vector<float>> inverseCurrentTensor = vector<vector<float>>();
		vector<float> firstRow = vector<float>(3);
		firstRow[0] = 1.f / currentTensor[0][0];
		vector<float> secondDRow = vector<float>(3);
		secondDRow[1] = 1.f / currentTensor[1][1];
		vector<float> thirdRow = vector<float>(3);
		thirdRow[2] = 1.f / currentTensor[2][2];
		inverseCurrentTensor.push_back(firstRow);
		inverseCurrentTensor.push_back(secondDRow);
		inverseCurrentTensor.push_back(thirdRow);

		// 7. Calculate angular velocity
		currentRigidBody.angularVelocity = Vec3(0.0);
		for (uint16_t i = 0; i < inverseCurrentTensor.size(); i++) {
			float result = 0;
			for (uint16_t j = 0; j < inverseCurrentTensor.size(); j++) {
				result += inverseCurrentTensor[i][j] * currentRigidBody.torque[j];
			}
			currentRigidBody.angularVelocity[i] = -result;
		}				*/


		currentRigidBody.angularVelocity -= Utility::MatrixVectorProduct3D(currentRigidBody.getTensorInWorldspace(), currentRigidBody.torque);
		checkCollisions();

		m_pRigidBodySystem->rigidBodies[i] = currentRigidBody;
	}
}

void RigidBodySystemSimulator::onClick(int x, int y) {

}

void RigidBodySystemSimulator::onMouse(int x, int y) {

}

// ExtraFunctions
int RigidBodySystemSimulator::getNumberOfRigidBodies() {
	return m_pRigidBodySystem->rigidBodies.size();
}

Vec3 RigidBodySystemSimulator::getPositionOfRigidBody(int i) {
	return m_pRigidBodySystem->rigidBodies[i].position;
}

Vec3 RigidBodySystemSimulator::getLinearVelocityOfRigidBody(int i) {
	return m_pRigidBodySystem->rigidBodies[i].linearVelocity;
}

Vec3 RigidBodySystemSimulator::getAngularVelocityOfRigidBody(int i) {
	return m_pRigidBodySystem->rigidBodies[i].angularVelocity;
}

void RigidBodySystemSimulator::applyForceOnBody(int i, Vec3 loc, Vec3 force) {
	m_pRigidBodySystem->applyForceOnBody(i, loc, force);
}

void RigidBodySystemSimulator::addRigidBody(Vec3 position, Vec3 size, int mass) {
	m_pRigidBodySystem->addRigidBody(position, size, mass);
}

void RigidBodySystemSimulator::setOrientationOf(int i, Quat orientation) {
	m_pRigidBodySystem->setOrientationOf(i, orientation);
}

void RigidBodySystemSimulator::setVelocityOf(int i, Vec3 velocity) {
	m_pRigidBodySystem->setVelocityOf(i, velocity);
}

void RigidBodySystemSimulator::checkCollisions()
{
	CollisionInfo info = checkCollisionSAT(m_pRigidBodySystem->rigidBodies[0].getObjectToWorld(), m_pRigidBodySystem->rigidBodies[1].getObjectToWorld());

	// collision detected
	if (info.isValid == 1)
	{
		// math formula for reflection along n: r = d−2(d⋅n)n
		Vec3 n = Utility::getNormalizedVector(info.normalWorld);
		Vec3 d = m_pRigidBodySystem->rigidBodies[0].linearVelocity;
		m_pRigidBodySystem->rigidBodies[0].linearVelocity = d - 2 * (Utility::scalarProduct(d, n))*n;
		applyForceOnBody(0, info.collisionPointWorld, m_pRigidBodySystem->rigidBodies[0].mass*n);

		n *= -1;
		d = m_pRigidBodySystem->rigidBodies[1].linearVelocity;
		m_pRigidBodySystem->rigidBodies[1].linearVelocity = d - 2 * (Utility::scalarProduct(d, n))*n;
		applyForceOnBody(1, info.collisionPointWorld, m_pRigidBodySystem->rigidBodies[1].mass*n);
	}
}