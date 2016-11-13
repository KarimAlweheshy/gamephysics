#include "MassSpringSystemSimulator.h"

enum Demo {
	DemoUnkown = 0,
	Demo1 = 1,
	Demo2 = 2,
	Demo3 = 3,
	Demo4 = 4
};

struct MassSpringSystem {
public:
	vector<vector<float>> springsMatrix;
	vector<MassPoint> massPoints;
	Vec3 externalForce;
};

MassSpringSystem massSpringSystem = MassSpringSystem();
Demo prevDemo = DemoUnkown;
Demo demo = Demo1;
Vec3 gravity = Vec3(0, 0, 0);

MassSpringSystemSimulator::MassSpringSystemSimulator() {
	SetupDemo1();
}


const char * MassSpringSystemSimulator::getTestCasesStr(){
	return "Euler,MidPoint";
}

void MassSpringSystemSimulator::reset(){
	SetupDemo1();
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	m_externalForce = Vec3();
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	TwAddVarRW(DUC->g_pTweakBar, "#Demo", TW_TYPE_INT32, &demo, "min=1 step=1 max=4");
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	if (prevDemo != demo) {
		switch (demo)
		{
		case Demo1:
			SetupDemo1();
			break;
		case Demo2: 
			SetupDemo2();
			break;
		case Demo3: 
			SetupDemo3();
			break;
		case Demo4: 
			SetupDemo4();
			break;
		}
		prevDemo = demo;
	}
	drawSingleSpringSystem();
}

void MassSpringSystemSimulator::SetupDemo1()
{
	// add the 2 given mass points
	gravity = Vec3(0, 0, 0);
	
	massSpringSystem = MassSpringSystem();
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), 10, false);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), 10, true);
	addSpring(0, 1, 1);
}
void MassSpringSystemSimulator::SetupDemo2()
{
	// add the 2 given mass points
	setIntegrator(EULER);
	SetupDemo1();
}

void MassSpringSystemSimulator::SetupDemo3()
{
	// add the 2 given mass points
	setIntegrator(MIDPOINT);
	SetupDemo1();
}


void MassSpringSystemSimulator::SetupDemo4()
{
	gravity = Vec3(0, -9.8, 0);
	// add the 2 given mass points
	massSpringSystem = MassSpringSystem();
	addMassPoint(Vec3(2, 3, 5), Vec3(-1, 3, 2), 10, false);
	addMassPoint(Vec3(6, 5, 1), Vec3(-1, -2, 5), 10, false);
	addMassPoint(Vec3(5, 1, 2), Vec3(2, 2, -3), 10, false);
	addMassPoint(Vec3(3, 2, 0), Vec3(4, 0, 1), 10, false);
	addMassPoint(Vec3(2, 4, 6), Vec3(1, 0, 4), 10, false);
	addMassPoint(Vec3(5, 5, 1), Vec3(2, -3, 0), 10, false);
	addMassPoint(Vec3(1, 0, 0), Vec3(4, 2, 5), 10, false);
	addMassPoint(Vec3(3, 1, 0), Vec3(-1, 0, -1), 10, false);
	addMassPoint(Vec3(6, 1, 1), Vec3(-1, 3, -3), 10, false);
	addMassPoint(Vec3(3, 2, 6), Vec3(2, -1, 2), 10, false);
	addSpring(0, 1, 2);
	addSpring(1, 3, 5);
	addSpring(3, 5, 6);
	addSpring(5, 4, 1);
	addSpring(4, 3, 6);
	addSpring(3, 6, 4);
	addSpring(6, 7, 2);
	addSpring(7, 9, 8);
	addSpring(9, 0, 4);
	addSpring(0, 8, 6);
	addSpring(8, 3, 1);
	addSpring(3, 6, 2);
	addSpring(6, 0, 4);
	addSpring(0, 3, 7);
	addSpring(3, 2, 1);
}

// Call this function to update the rendering of demo1, make sure to calculate the next frame before this
void MassSpringSystemSimulator::drawSingleSpringSystem()
{
	std::mt19937 eng;
	std::uniform_real_distribution<float> randCol(0.0f, 1.0f);

	vector<vector<float>> tempSpringMap = massSpringSystem.springsMatrix;
	// Draw Frame according to calculated new positions for the points
	for (uint16_t i = 0; i < tempSpringMap.size(); i++)
	{
		for (uint16_t j = 0; j < tempSpringMap[i].size(); j++) {
			if (tempSpringMap[i][j] != 0) {
				Vec3 fistMassPointPosition = massSpringSystem.massPoints[i]._Position;
				Vec3 secondMassPointPosition = massSpringSystem.massPoints[j]._Position;

				DUC->setUpLighting(Vec3(), 0.4*Vec3(1, 1, 1), 100, 0.6*Vec3(randCol(eng), randCol(eng), randCol(eng)));

				// draw spring
				DUC->beginLine();
				DUC->drawLine(fistMassPointPosition, Vec3(randCol(eng), randCol(eng), randCol(eng)), secondMassPointPosition, Vec3(0, 255, 255));
				DUC->endLine();

				// Avoid drawing lines twice ;)
				tempSpringMap[j][i] = 0;
			}
		}
	}

	// draw all points of spring
	for (uint16_t i = 0; i < massSpringSystem.massPoints.size(); i++) {
		DUC->drawSphere(massSpringSystem.massPoints[i]._Position, Vec3(0.02, 0.02, 0.02));
	}
}

// Changing integrators from GIU
void MassSpringSystemSimulator::notifyCaseChanged(int testCase) {
	setIntegrator(testCase);
}

// TimeElapsed is the time step since last the last calculations
void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	// calculate external acceleration (mouse input / gravity / etc.) here
}

void MassSpringSystemSimulator::addSpring(uint16_t masspoint1, uint16_t masspoint2, float initialLength)
{
	if (masspoint1 < massSpringSystem.massPoints.size() &&
		masspoint2 < massSpringSystem.massPoints.size() &&
		masspoint1 != masspoint2) {
		massSpringSystem.springsMatrix[masspoint1][masspoint2] = initialLength;
		massSpringSystem.springsMatrix[masspoint2][masspoint1] = initialLength;
	}
}

void MassSpringSystemSimulator::simulateTimestep(float timeStep)
{
	/* basic principle:
	1. calculate external & spring forces -> get acceleration
	2. update position with old position and old velocity(call appropriate masspoint function)
	3. update velocity with acceleration from 1.(call appropriate masspoint function)
	*/

	externalForcesCalculations(timeStep);

	switch (m_iTestCase)
	{
	case Simulator::Euler:
		oneStepCalculation(timeStep);
		break;
	case Simulator::MidPoint:
		oneStepCalculation(timeStep / 2);
		oneStepCalculation(timeStep / 2);
		break;
	}
}

void MassSpringSystemSimulator::oneStepCalculation(float timeStep)
{
	vector<MassPoint> tempMassPoint = massSpringSystem.massPoints;
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		Vec3 massPointAcceleration = gravity;
		for (int j = 0; j < getNumberOfMassPoints(); j++) {
			if (i != j && massSpringSystem.springsMatrix[i][j] != 0) {
				massPointAcceleration += calculateAcceleration(i, j);
			}
		}

		tempMassPoint[i].integratePositions(timeStep);
		tempMassPoint[i].integrateVelocity(timeStep, massPointAcceleration);
	}
	massSpringSystem.massPoints = tempMassPoint;
}

Vec3 MassSpringSystemSimulator::calculateAcceleration(uint16_t massPoint0Index, uint16_t massPoint1Index) {
	// Calculate currentLength
	Vec3 point0Position = massSpringSystem.massPoints[massPoint0Index]._Position;
	Vec3 point1Position = massSpringSystem.massPoints[massPoint1Index]._Position;
	
	float currentLength = (float)hypot(hypot(point0Position.x - point1Position.x, point0Position.y - point1Position.y), point0Position.z - point1Position.z);

	Vec3 currentLengthVector = point1Position - point0Position;
	
	float initialLength = massSpringSystem.springsMatrix[massPoint0Index][massPoint1Index];
	Vec3 initialLengthVector = initialLength * (currentLengthVector / currentLength);

	Vec3 acceleration = m_fStiffness * (currentLengthVector - initialLengthVector) / m_fMass;
	return acceleration;
}

void MassSpringSystemSimulator::onClick(int x, int y) {
	// Not applicable in exercice 1
}

void MassSpringSystemSimulator::onMouse(int x, int y) {
	// Not applicable in exercice 1
}

// Changing integrator from code (for test cases)
void MassSpringSystemSimulator::setIntegrator(int integrator) {
	if (integrator == 0 || integrator == 1) {
		m_iTestCase = integrator;
	}
}

int MassSpringSystemSimulator::getNumberOfSprings() {
	uint16_t numberOfSprings = 0;
	vector<vector<float>> tempMassSpringMatrix = massSpringSystem.springsMatrix;
	for (uint16_t i = 0; i < tempMassSpringMatrix.size(); i++) {
		for (uint16_t j = 0; j < tempMassSpringMatrix[i].size(); j++) {
			if (tempMassSpringMatrix[i][j] > 0) {
				numberOfSprings++;
				tempMassSpringMatrix[j][i] = 0;
			}
		}
	}
	return numberOfSprings;
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

int MassSpringSystemSimulator::addMassPoint(Vec3 position, Vec3 velocity, float mass, bool isFixed) 
{
	MassPoint newMassPoint = MassPoint(position, velocity, mass, isFixed);
	vector<float> newMassPointSpring = vector<float>(massSpringSystem.springsMatrix.size() + 1, 0.f);
	for (uint16_t i = 0; i < massSpringSystem.springsMatrix.size(); i++) {
		massSpringSystem.springsMatrix[i].push_back(0);
	}
	massSpringSystem.springsMatrix.push_back(newMassPointSpring);
	massSpringSystem.massPoints.push_back(newMassPoint);
	// I have no clue what should I return!! So I just return the index of the new added MassPoint
	// It is not tested in the test cases of exercice 1 anyway :)
	return massSpringSystem.massPoints.size() - 1;
}

int MassSpringSystemSimulator::getNumberOfMassPoints() {
	return massSpringSystem.massPoints.size();
}

Vec3 MassSpringSystemSimulator::getPositionOfMassPoint(int index) {
	return massSpringSystem.massPoints[index]._Position;
}

Vec3 MassSpringSystemSimulator::getVelocityOfMassPoint(int index) {
	return massSpringSystem.massPoints[index]._Velocity;
}


void MassSpringSystemSimulator::applyExternalForce(Vec3 force) {
	massSpringSystem.externalForce += force;
}
