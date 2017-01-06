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
	vector<vector<int>> checkGrid(SphereSystem * system, gridPlane plane);

private:
	int m_iSize;
	float m_fCellSize;
	vector<vector<vector<int>>> grid;

	void fillGrid(gridPlane plane);
	vector<int> getCurrentSphereGridPos(Sphere * sphere, gridPlane plane);
};

