#ifndef SPHSYSTEMSIMULATOR_h
#define SPHSYSTEMSIMULATOR_h
#include "Simulator.h"
//#include "spheresystem.h", add your sphere system header file
#include "Sphere.h"

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
	Vec3 m_fGravity;
	int   m_iNumSpheres;
	
	int   m_iKernel; // index of the m_Kernels[5], more detials in SphereSystemSimulator.cpp
	static std::function<float(float)> m_Kernels[5];
	
	int   m_iAccelerator; // switch between NAIVEACC and GRIDACC, (optionally, KDTREEACC, 2)
	
	enum collisionType{NAIVE, KD_TREE, GRID};

	struct SphereSystem {
	public:
		vector<Sphere> spheres;
		Vec3 externalForce;
		collisionType type;
		Vec3 color;
	};

	vector<SphereSystem> sphereSystems;

	void addSphereSystem(int n_Spheres, collisionType type);
	void checkCollisions(vector<Sphere> spheres);
	void checkBoundingBoxCollision(Sphere* sphere);
	vector<Sphere> eulerStepCalculation(vector<Sphere> spheres, float timeStep);
	vector<Sphere> midpointCalculations(vector<Sphere> oldSpheres, vector<Sphere> midpointSpheres, float timeStep);
	vector<Vec3>  calculateAccelerations(vector<Sphere> spheres);
	//SphereSystem * m_pSphereSystem; // add your own sphere system member!
	// for Demo 3 only:
	// you will need multiple SphereSystem objects to do comparisons in Demo 3
	// m_iAccelerator should be ignored.
	// SphereSystem * m_pSphereSystemGrid; 

};

#endif