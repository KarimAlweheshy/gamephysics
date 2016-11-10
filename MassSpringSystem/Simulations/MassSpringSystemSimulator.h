#ifndef MASSSPRINGSYSTEMSIMULATOR_h
#define MASSSPRINGSYSTEMSIMULATOR_h
#include "Simulator.h"
#include "Spring.h"
#include "MassPoint.h"

// Do Not Change
#define EULER 0
#define LEAPFROG 1
#define MIDPOINT 2
// Do Not Change


class MassSpringSystemSimulator:public Simulator{
public:
	//Construtors
	MassSpringSystemSimulator();
	
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void SetupDemo1();
	void reset();
	void drawSingleSpringSystem();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void computeElasticForces(float timeElapsed);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	// Specific Functions
	void setIntegrator(int integrator);
	void setMass(float mass);
	// Uniform stiffness to all springs
	void setStiffness(float stiffness);
	void setDampingFactor(float damping);
	// Adds a point to the system
	int addMassPoint(Vec3 position, Vec3 velocity, float mass, bool isFixed);
	// Adds a spring between the points of indexes masspoint1 and masspoint2 from the added MassPoints array
	void addSpring(uint16_t masspoint1, uint16_t masspoint2, float initialLength);
	int getNumberOfMassPoints();
	int getNumberOfSprings();
	Vec3 getPositionOfMassPoint(int index);
	Vec3 getVelocityOfMassPoint(int index);
	void applyExternalForce(Vec3 force);

private:
	// Data Attributes
	float m_fMass;
	float m_fStiffness;
	float m_fDamping;
	int m_iIntegrator;

	// UI Attributes
	Vec3 m_externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;

	//Private functions
	void integrateMassPointsPositions(float timeStep);
	void integratePositionsUsingEuler(float timeStep);
};
#endif