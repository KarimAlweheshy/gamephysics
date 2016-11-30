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
	
		applyForceOnBody(0, Vec3(0.3, 0.5, 0.25), Vec3(1.0, 1.0, 0.0));
	}
}

void RigidBodySystemSimulator::externalForcesCalculations(float timeElapsed) {

}

void RigidBodySystemSimulator::simulateTimestep(float timeStep) {
	for (uint16_t i = 0; i < m_pRigidBodySystem->rigidBodies.size(); i++) {
		//1. Calculate all forces on body
		vector<ForceOnRigidBody> forcesOnBody = m_pRigidBodySystem->forcesOnRigidBodyWithIndex(i);
		Vec3 sumOfForcesVectors = Vec3();
		for (uint16_t j = 0; j < forcesOnBody.size(); j++) {
			sumOfForcesVectors += forcesOnBody[j].force;
		}

		// 2. Calculate q
		RigidBody currentRigidBody = m_pRigidBodySystem->rigidBodies[i];
		Vec3 q = Vec3();
		for (uint16_t i = 0; i < forcesOnBody.size(); i++) {
			Vec3 currentX = forcesOnBody[i].loc - currentRigidBody.position;
			Vec3 currentForce = forcesOnBody[i].force;
			q += cross(currentX, currentForce);
		}

		// 3. Euler Step for center of mass position and velosity
		currentRigidBody.position = currentRigidBody.position + (timeStep * currentRigidBody.linearVelocity);
		Vec3 temp = timeStep * sumOfForcesVectors / currentRigidBody.mass;
		currentRigidBody.linearVelocity = currentRigidBody.linearVelocity + temp;

		// 4. Update quaternion
		Quat angularVelocity = Quat();
		angularVelocity.w = 0;
		angularVelocity.x = currentRigidBody.angularVelocity.x * currentRigidBody.orientation.x;
		angularVelocity.y = currentRigidBody.angularVelocity.y * currentRigidBody.orientation.y;
		angularVelocity.z = currentRigidBody.angularVelocity.z * currentRigidBody.orientation.z;
		currentRigidBody.orientation = currentRigidBody.orientation + (timeStep / 2.0 * angularVelocity);

		// 5. Calculate L
		currentRigidBody.torque = currentRigidBody.torque + (timeStep * q);

		// 6. Get inverse current inertia tensor
		vector<vector<double>> inverseCurrentTensor = currentRigidBody.currentInverseTensor();

		// 7. Calculate angular velocity
		currentRigidBody.angularVelocity = Vec3();
		for (uint16_t i = 0; i < inverseCurrentTensor.size(); i++) {
			float result = 0;
			for (uint16_t j = 0; j < inverseCurrentTensor.size(); j++) {
				result += inverseCurrentTensor[i][j] * currentRigidBody.torque[j];
			}
			currentRigidBody.angularVelocity[i] = result;
		}
	}

	//Reset all forces on ridig bodies
	m_pRigidBodySystem->resetForceSOnAllBodies();
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
