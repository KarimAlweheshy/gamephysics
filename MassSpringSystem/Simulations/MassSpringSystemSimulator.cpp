#include "MassSpringSystemSimulator.h"
#include <cmath>

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
	setIntegrator(LEAPFROG);
}


const char * MassSpringSystemSimulator::getTestCasesStr(){
	return "Euler,LeapFrog,MidPoint";
}

void MassSpringSystemSimulator::reset(){
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	m_externalForce = Vec3();
	gravity = Vec3(0, 0, 0);
}

void MassSpringSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	TwAddVarRW(DUC->g_pTweakBar, "#Demo", TW_TYPE_INT32, &demo, "min=1 step=1 max=4");
}

void MassSpringSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	if (prevDemo != demo) {
		reset();
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
	}
	drawSingleSpringSystem();
}

void MassSpringSystemSimulator::SetupDemo1()
{
	// add the 2 given mass points
	prevDemo = Demo1;
	setStiffness(40.f);
	setMass(10.f);
	massSpringSystem = MassSpringSystem();
	addMassPoint(Vec3(0, 0, 0), Vec3(-1, 0, 0), 10, false);
	addMassPoint(Vec3(0, 2, 0), Vec3(1, 0, 0), 10, false);
	addSpring(0, 1, 1);
}
void MassSpringSystemSimulator::SetupDemo2()
{
	// add the 2 given mass points
	SetupDemo1();
	setIntegrator(EULER);
	prevDemo = Demo2;
}

void MassSpringSystemSimulator::SetupDemo3()
{
	// add the 2 given mass points
	SetupDemo1();
	setIntegrator(MIDPOINT);
	prevDemo = Demo3;
}


void MassSpringSystemSimulator::SetupDemo4()
{
	prevDemo = Demo4;
	setStiffness(40.f);
	setMass(10.f);
	gravity = Vec3(0, -9.8, 0);
	// add the 2 given mass points
	massSpringSystem = MassSpringSystem();
	addMassPoint(Vec3(0, 0, 0), Vec3(-0.1, 0.3, 0.2), 10, false);
	addMassPoint(Vec3(0.4, 1, 0), Vec3(-.1, -.2, 0.5), 10, false);
	addMassPoint(Vec3(0, 0.4, 1), Vec3(.2, .2, -.3), 10, false);
	addMassPoint(Vec3(0.3, 0.2, 0), Vec3(.4, .0, .1), 10, false);
	addMassPoint(Vec3(0.2, 0.4, 0.6), Vec3(.1, .0, .4), 10, false);
	addMassPoint(Vec3(0.5, 0.5, 0.1), Vec3(.2, -.3, .0), 10, false);
	addMassPoint(Vec3(0.1, 0, 0), Vec3(.4, .2, .5), 10, false);
	addMassPoint(Vec3(0.3, 0.1, 0), Vec3(-.1, 0, -.1), 10, false);
	addMassPoint(Vec3(0.6, 0.1, 0.1), Vec3(-.1, .3, -.3), 10, false);
	addMassPoint(Vec3(0.3, 0.2, 0.6), Vec3(.2, -.1, .2), 10, false);
	addSpring(0, 1, 1.f);
	addSpring(1, 3, 0.5f);
	addSpring(3, 5, 0.8f);
	addSpring(5, 4, 1.f);
	addSpring(4, 3, 0.6f);
	addSpring(3, 6, 0.4f);
	addSpring(6, 7, 0.2f);
	addSpring(7, 9, 0.8f);
	addSpring(9, 0, 0.4f);
	addSpring(0, 8, 0.6f);
	addSpring(8, 3, 0.1f);
	addSpring(3, 6, 0.2f);
	addSpring(6, 0, 0.4f);
	addSpring(0, 3, 0.7f);
	addSpring(3, 2, 0.1f);
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
	if (testCase == 1) {
		SetupDemo1();
	}
}

// TimeElapsed is the time step since last the last calculations
void MassSpringSystemSimulator::externalForcesCalculations(float timeElapsed)
{
	// Apply accumulated mouse deltas to g_vfMovableObjectPos (move along cameras view plane)
	if (m_trackmouse.x != 0 || m_trackmouse.y != 0)
	{
		// Calcuate camera directions in world space
		Mat4 m = Mat4(DUC->g_camera.GetViewMatrix());
		m = m.inverse();
		Vec3 camRightWorld = Vec3(g_XMIdentityR0);
		camRightWorld = m.transformVectorNormal(camRightWorld);
		Vec3 camUpWorld = Vec3(g_XMIdentityR1);
		camUpWorld = m.transformVectorNormal(camUpWorld);

		// Add accumulated mouse deltas to movable object pos

		float speedScale = 0.001f;
		for(int i = 0; i < getNumberOfMassPoints(); i++)
		{ 
			massSpringSystem.massPoints[i]._Position += speedScale * (float)m_trackmouse.x * camRightWorld;
			massSpringSystem.massPoints[i]._Position += -speedScale * (float)m_trackmouse.y * camUpWorld;
		}
		// Reset accumulated mouse deltas
		m_trackmouse.x = m_trackmouse.y = 0;
	}
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

	if (m_iTestCase == EULER) {
		massSpringSystem.massPoints = eulerStepCalculation(massSpringSystem.massPoints, timeStep);
	}
	else if (m_iTestCase == MIDPOINT) {
		vector<MassPoint> halfSetpMassPoints = eulerStepCalculation(massSpringSystem.massPoints, timeStep / 2);
		massSpringSystem.massPoints = midpointCalculations(massSpringSystem.massPoints, halfSetpMassPoints, timeStep);
	}
	else if (m_iTestCase == LEAPFROG) {
		//no initial drift for velocities but doesn't matter..
		vector<MassPoint> oneStepMassPoints = eulerStepCalculation(massSpringSystem.massPoints, timeStep);
		massSpringSystem.massPoints = leapFrogCalculations(massSpringSystem.massPoints, oneStepMassPoints, timeStep);
	}
	else {
		m_iTestCase = EULER;
	}
}

vector<MassPoint> MassSpringSystemSimulator::eulerStepCalculation(vector<MassPoint> massPoints, float timeStep)
{
	vector<MassPoint> tempMassPoint = massPoints;
	vector<Vec3> accelerations = calculateAccelerations(massPoints);

	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		tempMassPoint[i].integratePositions(timeStep);
		checkCollisions(i);
	}

	for (int i = 0; i < getNumberOfMassPoints(); i++) {
		tempMassPoint[i].integrateVelocity(timeStep, accelerations[i]);
	}
	
	return tempMassPoint;
}

vector<MassPoint> MassSpringSystemSimulator::midpointCalculations(vector<MassPoint> oldMassPoints, vector<MassPoint> midPointMassPoints, float timeStep) {
	vector<MassPoint> newMassPoints = oldMassPoints;
	vector<Vec3> midPointAccelerations = calculateAccelerations(midPointMassPoints);

	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		if (!newMassPoints[i]._IsFixed) {
			newMassPoints[i]._Position = oldMassPoints[i]._Position + (timeStep * midPointMassPoints[i]._Velocity);
		}
		checkCollisions(i);
	}
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		if (!newMassPoints[i]._IsFixed) {
			newMassPoints[i]._Velocity = oldMassPoints[i]._Velocity + (timeStep * midPointAccelerations[i]);
		}
	}
	return newMassPoints;
}

vector<MassPoint> MassSpringSystemSimulator::leapFrogCalculations(vector<MassPoint> oldMassPoints, vector<MassPoint> oneSetpMassPoints, float timeStep) {
	vector<MassPoint> newMassPoints = oldMassPoints;
	vector<Vec3> oldPointsAcceleration = calculateAccelerations(oldMassPoints);

	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		if (!newMassPoints[i]._IsFixed) {
			newMassPoints[i]._Position = oldMassPoints[i]._Position + (timeStep * oldMassPoints[i]._Velocity) + (0.5 * oldPointsAcceleration[i] * pow(timeStep, 2));
		}
		checkCollisions(i);
	}

	vector<Vec3> newPointsAcceleration = calculateAccelerations(newMassPoints);

	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		if (!newMassPoints[i]._IsFixed) {
			newMassPoints[i]._Velocity = oldMassPoints[i]._Velocity + (timeStep / 2 * (oldPointsAcceleration[i] + newPointsAcceleration[i]));
		}
	}
	return newMassPoints;
}

vector<Vec3> MassSpringSystemSimulator::calculateAccelerations(vector<MassPoint> massPoints) {
	vector<Vec3> accelerations = vector<Vec3>();
	for (int i = 0; i < getNumberOfMassPoints(); i++)
	{
		accelerations.push_back(gravity);
		for (uint16_t j = 0; j < massPoints.size(); j++) {
			if (i != j && massSpringSystem.springsMatrix[i][j] != 0) {
				accelerations[i] += calculateAcceleration(massPoints, i, j);
			}
		}
	}
	return accelerations;
}

Vec3 MassSpringSystemSimulator::calculateAcceleration(vector<MassPoint> massPoints, uint16_t massPoint0Index, uint16_t massPoint1Index) {
	
	// Calculate currentLength
	Vec3 point0Position = massPoints[massPoint0Index]._Position;
	Vec3 point1Position = massPoints[massPoint1Index]._Position;
	
	float currentLength = Utility::getVectorLength(point1Position - point0Position);
	Vec3 currentLengthVector = Utility::getNormalizedVector(point1Position - point0Position);
	float initialLength = massSpringSystem.springsMatrix[massPoint0Index][massPoint1Index];
	Vec3 acceleration = -m_fStiffness * (currentLength - initialLength) / m_fMass * Utility::getNormalizedVector(point0Position - point1Position);

	return acceleration;	
}

// Changing integrator from code (for test cases)
void MassSpringSystemSimulator::setIntegrator(int integrator) {
	if (integrator == EULER || integrator == MIDPOINT || integrator == LEAPFROG) {
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


// if y position is < 0 -> reflect
void MassSpringSystemSimulator::checkCollisions(uint16_t massPointId)
{
	Vec3 tmpPos = getPositionOfMassPoint(massPointId);
	if (tmpPos.Y < -1.f)
	{ 
		massSpringSystem.massPoints[massPointId]._Position.y = -1.f - 0.1*tmpPos.Y;
		massSpringSystem.massPoints[massPointId]._Velocity = Utility::reverseVectorY(massSpringSystem.massPoints[massPointId]._Velocity);
	}
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

void MassSpringSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x += x - m_oldtrackmouse.x;
	m_trackmouse.y += y - m_oldtrackmouse.y;
}

void MassSpringSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
};