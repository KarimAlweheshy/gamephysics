#include "SphereSystemSimulator.h"
#include "Utility.h"

std::function<float(float)> SphereSystemSimulator::m_Kernels[5] = {
	[](float x) {return 1.0f; },              // Constant, m_iKernel = 0
	[](float x) {return 1.0f - x; },          // Linear, m_iKernel = 1, as given in the exercise Sheet, x = d/2r
	[](float x) {return (1.0f - x)*(1.0f - x); }, // Quadratic, m_iKernel = 2
	[](float x) {return 1.0f / (x)-1.0f; },     // Weak Electric Charge, m_iKernel = 3
	[](float x) {return 1.0f / (x*x) - 1.0f; },   // Electric Charge, m_iKernel = 4
};

// SphereSystemSimulator member functions
SphereSystemSimulator::SphereSystemSimulator()
{
	m_fRadius = 0.005f;
	m_fMass = 0.1f;
	m_fGravity = Vec3(0.0f, -9.81f, 0.0f);
	m_fDamping = 0.75;
}

const char * SphereSystemSimulator::getTestCasesStr(){
	return "Naive,Accelerated,Performance";
}

void SphereSystemSimulator::reset(){
	m_mouse.x = m_mouse.y = 0;
	m_trackmouse.x = m_trackmouse.y = 0;
	m_oldtrackmouse.x = m_oldtrackmouse.y = 0;
	externalForce = Vec3();
	sphereSystems.clear();
}

void SphereSystemSimulator::initUI(DrawingUtilitiesClass * DUC)
{
	this->DUC = DUC;
	TwAddVarRW(DUC->g_pTweakBar, "Sphere Radius", TW_TYPE_FLOAT, &m_fRadius, "min=0.005 step=0.001 max=0.05");
	TwAddVarRW(DUC->g_pTweakBar, "Sphere Mass", TW_TYPE_FLOAT, &m_fMass, "min=0.01 step=0.05 max=2"); 
}

void SphereSystemSimulator::drawFrame(ID3D11DeviceContext* pd3dImmediateContext)
{
	for (int i = 0; i < sphereSystems.size(); i++)
	{
		for (int j = 0; j < sphereSystems[i].spheres.size(); j++)
			DUC->drawSphere(sphereSystems[i].spheres[j]._Position, Vec3(m_fRadius, m_fRadius, m_fRadius));
	}
}

void SphereSystemSimulator::notifyCaseChanged(int testCase) 
{
	// could be changed to acceleration = 1/(m*d²)for realism
	switch (testCase)
	{
	case 0:
		addSphereSystem(100, collisionType::NAIVE);
		break;
	case 1:
		addSphereSystem(100, collisionType::GRID);
		break;
	case 2: break;

	}
}

// TimeElapsed is the time step since last the last calculations
void SphereSystemSimulator::externalForcesCalculations(float timeElapsed)
{	
}

void SphereSystemSimulator::simulateTimestep(float timeStep)
{
	m_fTimestep = timeStep;

	for (int i = 0; i < sphereSystems.size(); i++)
	{
		SphereSystem oldSystem = sphereSystems[i];
		eulerStepCalculation(i, timeStep / 2);
		midpointCalculations(&oldSystem, i, timeStep);
	}
}

void SphereSystemSimulator::onClick(int x, int y)
{
	m_trackmouse.x += x - m_oldtrackmouse.x;
	m_trackmouse.y += y - m_oldtrackmouse.y;
}

void SphereSystemSimulator::onMouse(int x, int y)
{
	m_oldtrackmouse.x = x;
	m_oldtrackmouse.y = y;
};

void SphereSystemSimulator::addSphereSystem(int n_Spheres, collisionType type)
{
	SphereSystem system;
	system.type = type;
	std::mt19937 eng;
	std::uniform_real_distribution<float> randCol(0.0f, 1.0f);
	system.color = 0.5*Vec3(randCol(eng), randCol(eng), randCol(eng));
	
	for (int i = 0; i < n_Spheres; i++)
	{
		Vec3 randPos(Utility::getRandomFloat(-0.5f, 0.5f), Utility::getRandomFloat(-.5f, .5f), Utility::getRandomFloat(-0.5f, 0.5f));
		Vec3 randVel(Utility::getRandomFloat(-1.0f, 1.0f), Utility::getRandomFloat(-1.0f, 1.0f), Utility::getRandomFloat(-1.0f, 1.0f));

		system.spheres.push_back(Sphere(randPos, randVel));
	}

	sphereSystems.push_back(system);
}

void SphereSystemSimulator::eulerStepCalculation(int i_SphereSystem, float timeStep)
{
	vector<Vec3> accelerations = calculateAccelerations(i_SphereSystem, true);

	for (int i = 0; i < sphereSystems[i_SphereSystem].spheres.size(); i++)
		sphereSystems[i_SphereSystem].spheres[i].integratePositions(timeStep);

	for (int i = 0; i < sphereSystems[i_SphereSystem].spheres.size(); i++)
		sphereSystems[i_SphereSystem].spheres[i].integrateVelocity(timeStep, accelerations[i]);
}

void SphereSystemSimulator::midpointCalculations(SphereSystem * old_System, int i_MidpointSystem, float timeStep)
{
	vector<Vec3> midPointAccelerations = calculateAccelerations(i_MidpointSystem, true);

	for (int i = 0; i < sphereSystems[i_MidpointSystem].spheres.size(); i++)
		sphereSystems[i_MidpointSystem].spheres[i]._Position = old_System->spheres[i]._Position + (timeStep * sphereSystems[i_MidpointSystem].spheres[i]._Velocity);

	for (int i = 0; i < sphereSystems[i_MidpointSystem].spheres.size(); i++)
		sphereSystems[i_MidpointSystem].spheres[i]._Velocity = old_System->spheres[i]._Velocity + (timeStep * midPointAccelerations[i]);

}

vector<Vec3> SphereSystemSimulator::calculateAccelerations(int i_SphereSystem, bool withCollisions)
{
	vector<Vec3> accelerations;

	for (int i = 0; i < sphereSystems[i_SphereSystem].spheres.size(); i++)
		accelerations.push_back(m_fGravity);

	if (withCollisions)
		checkCollisions(i_SphereSystem, &accelerations);

	return accelerations;
}

void SphereSystemSimulator::checkCollisions(int i_SphereSystem, vector<Vec3> * accelerations)
{
	switch (sphereSystems[i_SphereSystem].type)
	{
	case collisionType::NAIVE:
		naiveCollision(i_SphereSystem, accelerations);
		break;

	case collisionType::KD_TREE:
		break;

	case collisionType::GRID:
		gridCollision(i_SphereSystem, accelerations);
		break;
	}
}

/*
 Summary: Bounding Box = Magnetic Field

 Tested several different approaches. Just reversing the velocity along 1 axis does not work when the collision detection is done in the acceleration
 calculation -> the sphere will just fluctuate around the border. Using a velocity-based acceleration leads to instable results. The workarund that 
 was chosen with a large, static acceleration works somewhat like a magnetic field.
*/
Vec3 SphereSystemSimulator::checkBoundingBoxCollision(Sphere * sphere)
{
	Vec3 acceleration(0, 0, 0);

	for (int i = 0; i < 3; i++)
	{
		float distance = abs(sphere->_Position[i]) - 0.5;
		if (sphere->_Position[i] <= -0.5)
		{	
			acceleration[i] = 80;
		}
		else if (sphere->_Position[i] >= 0.5)
		{
			acceleration[i] = -85;
		}
	}

	return acceleration;
}

/*
 Test every sphere against every other sphere (in the same system)
*/
void SphereSystemSimulator::naiveCollision(int i_SphereSystem, vector<Vec3> * accelerations)
{
	for (int i = 0; i < sphereSystems[i_SphereSystem].spheres.size(); i++)
	{
		// check bounding box collisions
		accelerations->at(i) += checkBoundingBoxCollision(&sphereSystems[i_SphereSystem].spheres[i]);

		for (int j = 0; j < sphereSystems[i_SphereSystem].spheres.size(); j++)
		{
			// skip collision detection for current sphere
			if (i == j)
				continue;

			float distance = Utility::getVectorDistance(sphereSystems[i_SphereSystem].spheres[i]._Position, sphereSystems[i_SphereSystem].spheres[j]._Position);
			//if(collision i & j) -> add acceleration to i (j will be taken care of in its own loop)
			if (distance <= m_fRadius)
			{
				// j - i to get the direction of force for i
				Vec3 direction = Utility::getNormalizedVector(sphereSystems[i_SphereSystem].spheres[j]._Position - sphereSystems[i_SphereSystem].spheres[i]._Position);
				float force = (1.0f - distance / (2 * m_fRadius));
				accelerations->at(i) += force / m_fMass*direction;
			}
		}
	}
}

void SphereSystemSimulator::gridCollision(int i_SphereSystem, vector<Vec3> * accelerations)
{
	/*
	 1. Get index matrix from UniformGrid (not finished)
	 2. Calculate collisions according to index matrix
	*/
	
	// size = 1.4 to take fuzzy edges into consideration
	int size = (1.4 / (2 * m_fRadius));
	UniformGrid grid(size,  2 * m_fRadius);
	vector<vector<int>> indexMatrix = grid.checkGrid(sphereSystems[i_SphereSystem]);

	for (int i = 0; i < sphereSystems[i_SphereSystem].spheres.size(); i++)
	{
		// check bounding box collisions
		accelerations->at(i) += checkBoundingBoxCollision(&sphereSystems[i_SphereSystem].spheres[i]);
		
		for (int j = 0; j < indexMatrix[i].size(); j++)
		{
			float distance = Utility::getVectorDistance(sphereSystems[i_SphereSystem].spheres[i]._Position, sphereSystems[i_SphereSystem].spheres[indexMatrix[i][j]]._Position);
			Vec3 direction = Utility::getNormalizedVector(sphereSystems[i_SphereSystem].spheres[indexMatrix[i][j]]._Position - sphereSystems[i_SphereSystem].spheres[i]._Position);
			float force = (1.0f - distance / (2 * m_fRadius));
			accelerations->at(i) += force / m_fMass*direction;
		}
		
	}
}
