#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h
#include "Simulator.h"
//#include "spheresystem.h", add your sphere system header file
#include "SphereSystem.h"
#include "Sphere.h"
#include "UniformGrid.h"
#include "config.h"

#define NAIVEACC 0
#define GRIDACC 1

class SphereSystemSimulator:public Simulator{
public:
	// Construtors
	SphereSystemSimulator();
	// Functions
	const char * getTestCasesStr();
	void initUI(DrawingUtilitiesClass * DUC);
	void reset();
	void drawFrame(ID3D11DeviceContext* pd3dImmediateContext);
	void notifyCaseChanged(int testCase);
	void externalForcesCalculations(float timeElapsed);
	void simulateTimestep(float timeStep);
	void onClick(int x, int y);
	void onMouse(int x, int y);
	
protected:
	// Attributes
	Vec3 externalForce;
	Point2D m_mouse;
	Point2D m_trackmouse;
	Point2D m_oldtrackmouse;
	float m_fMass;
	float m_fRadius;
	float m_fForceScaling;
	float m_fDamping;
	float m_fTimestep;
	Vec3  m_fGravity;
	int   m_iNumSpheres;
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)

	vector<SphereSystem> sphereSystems;

	void addSphereSystem(int n_Spheres, collisionType type);
	void checkCollisions(int i_SphereSystem, vector<Vec3> * accelerations);
	void naiveCollision(int i_SphereSystem, vector<Vec3> * accelerations);
	void gridCollision(int i_SphereSystem, vector<Vec3> * accelerations);
	void eulerStepCalculation(int i_SphereSystem, float timeStep);
	void midpointCalculations(SphereSystem * old_System, int i_MidpointSystem, float timeStep);
	Vec3 checkBoundingBoxCollision(Sphere *	sphere);
	vector<Vec3>  calculateAccelerations(int i_SphereSystem, bool withCollisions);
	//SphereSystem * m_pSphereSystem; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	// SphereSystem * m_pSphereSystemGrid; 
	
};

#endif