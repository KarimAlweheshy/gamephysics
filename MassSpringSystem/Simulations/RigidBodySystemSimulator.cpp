#include "RigidBodySystemSimulator.h"

// Construtors
RigidBodySystemSimulator::RigidBodySystemSimulator() {
	notifyCaseChanged(1);
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

		Vec3 position = Vec3(1.0);
		Vec3 size = Vec3(1, 0.6, 0.5);
		int mass = 2;
		addRigidBody(position, size, mass);

		Quat quat = Quat(Vec3(0, 0, 1), 90);
		setOrientationOf(0, quat);
	}
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed) {

}

void RigidBodySystemSimulator::simulateTimestep(float timeStep) {
	/* basic principle:
	1. calculate current tensor
	2. update position with old position and old velocity(call appropriate masspoint function)
	3. update velocity with acceleration from 1.(call appropriate masspoint function)
	*/
	vector<vector<double>> currentTensor = m_pRigidBodySystem->rigidBodies[0].currentTensor();
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
