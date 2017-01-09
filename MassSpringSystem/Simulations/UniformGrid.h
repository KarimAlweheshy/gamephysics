#pragma once

#include "Simulator.h"
#include "SphereSystem.h"
#include "Sphere.h"

class UniformGrid
{
public:
	UniformGrid(int i_width, float f_cellWidth);

	/*
	 Returns an index matrix. The first entry of each row is the target sphere index, 
	 the following row entries are collision sphere indices.
	*/
	vector<vector<int>> checkGrid(SphereSystem& system);

private:
	int m_iSize;
	float m_fCellSize;

	struct Grids
	{
	public:
		vector<vector<int>> gridX;
		vector<vector<int>> gridY;
		vector<vector<int>> gridZ;
	};

	Grids uniformGrids;

	void fillGrids(SphereSystem& system);
	void clearGrids();
	vector<int> getSurroundingSpheres(Sphere& currentSphere, int sphereIndex);
	Vec3 getCurrentSphereGridPos(Sphere& sphere);
};

