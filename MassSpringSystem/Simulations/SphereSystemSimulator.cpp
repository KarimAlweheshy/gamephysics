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
	m_fGravity = Vec3(0.0f, -4.81f, 0.0f);
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
	switch (testCase)
	{
	case 0:
		addSphereSystem(100, collisionType::NAIVE);
		break;
	case 1:
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
	for (int i = 0; i < sphereSystems.size(); i++)
	{
		checkCollisions(&sphereSystems[i].spheres);
		vector<Sphere> halfstepSpheres = eulerStepCalculation(sphereSystems[i].spheres, timeStep / 2);
		sphereSystems[i].spheres = midpointCalculations(sphereSystems[i].spheres, halfstepSpheres, timeStep);
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

vector<Sphere> SphereSystemSimulator::eulerStepCalculation(vector<Sphere> spheres, float timeStep)
{
	vector<Sphere> tmpSpheres = spheres;
	vector<Vec3> accelerations = calculateAccelerations(spheres);

	for (int i = 0; i < spheres.size(); i++)
	{
		tmpSpheres[i].integratePositions(timeStep);
	}

	for (int i = 0; i < spheres.size(); i++)
	{
		tmpSpheres[i].integrateVelocity(timeStep, accelerations[i]);
	}

	return tmpSpheres;
}

vector<Sphere> SphereSystemSimulator::midpointCalculations(vector<Sphere> oldSpheres, vector<Sphere> midpointSpheres, float timeStep)
{
	vector<Sphere> newSpheres = oldSpheres;
	vector<Vec3> midPointAccelerations = calculateAccelerations(midpointSpheres);

	for (int i = 0; i < oldSpheres.size(); i++)
	{
		newSpheres[i]._Position = oldSpheres[i]._Position + (timeStep * midpointSpheres[i]._Velocity);
	}

	for (int i = 0; i < oldSpheres.size(); i++)
	{
		newSpheres[i]._Velocity = oldSpheres[i]._Velocity + (timeStep * midPointAccelerations[i]);
	}

	return newSpheres;
}

vector<Vec3> SphereSystemSimulator::calculateAccelerations(vector<Sphere> spheres)
{
	vector<Vec3> accelerations;
	
	for (int i = 0; i < spheres.size(); i++)
		accelerations.push_back(m_fGravity);

	return accelerations;
}

void SphereSystemSimulator::checkCollisions(vector<Sphere> * spheres)
{
	for (int i = 0; i < spheres->size(); i++)
		checkBoundingBoxCollision(&spheres->at(i));

	/*switch (system.type)
	{
	case collisionType::NAIVE:
		break;

	case collisionType::KD_TREE:
		break;

	case collisionType::GRID:
		break;
	}*/
}

void SphereSystemSimulator::checkBoundingBoxCollision(Sphere * sphere)
{
	for (int i = 0; i < 3; i++)
	{
		if (sphere->_Position[i] <= -0.5)
		{
			sphere->_Position[i] = -0.49;
			sphere->_Velocity[i] *= -1;
			//cout << "Pos: " << sphere._Position[i] << "| Vel: " << sphere._Velocity[i] << endl;
		}
		else if (sphere->_Position[i] >= 0.5)
		{
			sphere->_Position[i] = 0.49;
			sphere->_Velocity[i] *= -1;
		}

	}

}