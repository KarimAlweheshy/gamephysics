#include "MassSpringSystemSimulator.h"

enum Demo {
	Demo1 = 0,
	Demo2 = 1,
	Demo3 = 2,
	Demo4 = 3
};

/* Commented in case we need the structs again later
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
*/

struct MassSpringSystem {
public:
	vector<vector<float>> springsMatrix;
	vector<MassPoint> massPoints;
	Vec3 externalForce;
};

MassSpringSystem massSpringSystem = MassSpringSystem();
int numberOfPoints = 2;
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
	TwAddVarRW(DUC->g_pTweakBar, "#Mass points", TW_TYPE_INT32, &numberOfPoints, "min=2");
	switch (m_iTestCase)
	{
	case 0:
		SetupDemo1();
		break;
	case 1:break;
	case 2:break;
	default:break;
	}
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	switch (m_iTestCase)
	{
	case 0:
		drawSingleSpringSystem();
		break;

	case 1: break;
	case 2: break;
	case 3: break;
	}
}

void MassSpringSystemSimulator::SetupDemo1()
{
	// add the 2 given mass points
	m_fStiffness = 40;
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), 10, false);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), 10, false);
	addSpring(0, 1, 1);
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

void MassSpringSystemSimulator::notifyCaseChanged(int testCase) {
	if (testCase >= 0 && testCase <= 3) {
		demo = Demo(testCase);
	}
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

	switch (m_iIntegrator)
	{
	case Simulator::Euler:
		oneStepCalculation(timeStep);
		break;
	case Simulator::MidPoint:
		oneStepCalculation(timeStep / 2);
		oneStepCalculation(timeStep / 2);
		break;
	}

	//integrateMassPointsPositions(timeStep);
}

void MassSpringSystemSimulator::oneStepCalculation(float timeStep)
{
	vector<MassPoint> tempMassPoint = massSpringSystem.massPoints;
	Vec3 gravity = Vec3(0, -9.8, 0);
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		Vec3 massPointAcceleration = gravity;
		for (int j = 0; j < getNumberOfMassPoints(); j++) {
			if (i != j && massSpringSystem.springsMatrix[i][j] != 0) {
				massPointAcceleration += calculateAcceleration(i, j);
			}
		}

		tempMassPoint[i].integratePositions(timeStep, m_iIntegrator);
		tempMassPoint[i].integrateVelocity(timeStep, m_iIntegrator, massPointAcceleration);
	}
	massSpringSystem.massPoints = tempMassPoint;
}

Vec3 MassSpringSystemSimulator::calculateAcceleration(uint16_t massPoint0Index, uint16_t massPoint1Index) {
	// Calculate currentLength
	MassPoint point1 = massSpringSystem.massPoints[massPoint0Index];
	MassPoint point2 = massSpringSystem.massPoints[massPoint1Index];
	float currentLength = (float)hypot(hypot(point1._Position.x - point1._Position.x, point1._Position.y - point2._Position.y), point1._Position.z - point2._Position.z);

	Vec3 currentLengthVector = massSpringSystem.massPoints[massPoint1Index]._Position - massSpringSystem.massPoints[massPoint0Index]._Position;
	float initialLength = massSpringSystem.springsMatrix[massPoint0Index][massPoint1Index];
	Vec3 initialLengthVector = initialLength * (currentLengthVector / currentLength);

	return - m_fStiffness * (currentLengthVector - initialLengthVector) / m_fMass;
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
		m_iIntegrator = integrator;
	}
}

int MassSpringSystemSimulator::getNumberOfSprings() {
	uint16_t numberOfSprings = 0;
	for (uint16_t i = 0; i < massSpringSystem.springsMatrix.size(); i++) {
		for (uint16_t j = 0; j < massSpringSystem.springsMatrix[i].size(); j++) {
			if (massSpringSystem.springsMatrix[i][j] > 0) {
				numberOfSprings++;
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
