#include "MassSpringSystemSimulator.h"

enum Integrator { 
	Euler = 0,
	MidPoint = 1 
};

enum Demo {
	Demo1 = 0,
	Demo2 = 1,
	Demo3 = 2,
	Demo4 = 3
};

struct MassPoint {
public:
	Vec3 position;
	Vec3 velosity;
	Vec3 acceleration;
	int mass;
	bool isFixed;
	vector<int> attachedToPoints;
};

struct Spring {
public:
	MassPoint point1;
	MassPoint point2;
	float initialLength;
};

struct MassSpringSystem {
public:
	vector<MassPoint> massPoints;
	vector<Spring> springs;
	Vec3 externalForce;
};

MassSpringSystem massSpringSystem = MassSpringSystem();
int numberOfPoints = 2;
Integrator integrator = Euler;
Demo demo = Demo1;

MassSpringSystemSimulator::MassSpringSystemSimulator()
{
	m_iTestCase = 0;
	m_fStiffness = 40;
}

const char * MassSpringSystemSimulator::getTestCasesStr(){
	return "Euler,MidPoint";
}

void MassSpringSystemSimulator::reset(){
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	m_externalForce = Vec3();
	m_iIntegrator = 0;
	m_fMass = 10;
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	switch (m_iTestCase)
	{
	case 0:break;
	case 1:
		TwAddVarRW(DUC->g_pTweakBar, "Num Points", TW_TYPE_INT32, &numberOfPoints, "min=2");
		break;
	case 2:break;
	default:break;
	}
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	// Draw Frame according to calculated new positions for the points 
}

void MassSpringSystemSimulator::notifyCaseChanged(int testCase) {
	if (testCase >= 0 && testCase <= 3) {
		demo = Demo(testCase);
	}
}

// TimeElapsed is the time step since last the last calculations
void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed) {
	switch (integrator) {
	case Euler:
		// Calculate using Euler method
		break;
	case MidPoint:
		// Calculate using MidPoint method
		break;
	}
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep) {
	// Not applicable in exercice 1
}

void MassSpringSystemSimulator::onClick(int x, int y) {
	// Not applicable in exercice 1
}

void MassSpringSystemSimulator::onMouse(int x, int y) {
	// Not applicable in exercice 1
}

void MassSpringSystemSimulator::setIntegrator(int integrator) {
	// No clue what is this :D
	if (integrator == 0 || integrator == 1) {
		this->setIntegrator(Integrator(integrator));
	}
}

void MassSpringSystemSimulator::setMass(float mass) {
	m_fMass = mass;
}

void MassSpringSystemSimulator::setStiffness(float stiffness) {
	m_fStiffness = stiffness;
}

void MassSpringSystemSimulator::setDampingFactor(float damping) {
	m_fDamping = damping;
}

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 Velocity, bool isFixed) {
	MassPoint newMassPoint = MassPoint();
	newMassPoint.position = position;
	newMassPoint.velosity = Velocity;
	newMassPoint.acceleration = isFixed;
	massSpringSystem.massPoints.push_back(newMassPoint);
	// I have no clue what should I return!! So I just return the index of the new added MassPoint
	// It is not tested in the test cases of exercice 1 anyway :)
	return massSpringSystem.massPoints.size() - 1;
}

void MassSpringSystemSimulator::addSpring(int masspoint1, int masspoint2, float initialLength) {
	Spring spring = Spring();
	spring.point1 = massSpringSystem.massPoints[masspoint1];
	spring.point2 = massSpringSystem.massPoints[masspoint1];
	spring.initialLength = initialLength;
	massSpringSystem.springs.push_back(spring);
}

int MassSpringSystemSimulator::getNumberOfMassPoints() {
	return massSpringSystem.massPoints.size();
}

int MassSpringSystemSimulator::getNumberOfSprings() {
	return massSpringSystem.springs.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) {
	return massSpringSystem.massPoints[index].position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) {
	return massSpringSystem.massPoints[index].velosity;
}

void MassSpringSystemSimulator::applyExternalForce(Vec3 force) {
	massSpringSystem.externalForce += force;
}
