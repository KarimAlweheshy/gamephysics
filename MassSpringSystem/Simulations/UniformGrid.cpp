#include "UniformGrid.h"

UniformGrid::UniformGrid(int i_width, float f_cellWidth)
{
	m_iSize = i_width;
	m_fCellSize = f_cellWidth;
}

vector<vector<int>> UniformGrid::checkGrid(SphereSystem& system)
{
	clearGrids();
	fillGrids(system);

	vector<vector<int>> indexMatrix;
	/*
	// for all spheres:
	// check grids at surrounding positions (3 checks: -1, 0, 1)
	// if another sphere is within margin on all 3 axes -> add to indexMatrix vector at sphere position
	*/
	for (int i = 0; i < system.spheres.size(); i++)
		indexMatrix.push_back(getSurroundingSpheres(system.spheres[i],i));
	
	return indexMatrix;
}

Vec3 UniformGrid::getCurrentSphereGridPos(Sphere& sphere)
{
	Vec3 pos;

	// need to increase positon by 0.5 to get [0,1] range. (actually need to increase by more since bounding box is fuzzy
	for (int i = 0; i < 3; i++)
		pos[i] = (int)( (sphere._Position[i] + 0.7f) / m_fCellSize);
		
	return pos;
}

void UniformGrid::fillGrids(SphereSystem& system)
{
	vector<int> entry;
	// fill grids up to size
	for (int i = 0; i <= m_iSize; i++)
	{
		uniformGrids.gridX.push_back(entry);
		uniformGrids.gridY.push_back(entry);
		uniformGrids.gridZ.push_back(entry);
	}

	for (int i = 0; i < system.spheres.size(); i++)
	{
		Vec3 sphereGridPos = getCurrentSphereGridPos(system.spheres[i]);
		uniformGrids.gridX[sphereGridPos[0]].push_back(i);
		uniformGrids.gridY[sphereGridPos[1]].push_back(i);
		uniformGrids.gridZ[sphereGridPos[2]].push_back(i);
	}
}

void UniformGrid::clearGrids()
{
	uniformGrids.gridX.clear();
	uniformGrids.gridY.clear();
	uniformGrids.gridZ.clear();
}


// TODO: Include check for edges -> cant do +1/-1 at edges
vector<int> UniformGrid::getSurroundingSpheres(Sphere& currentSphere, int sphereIndex)
{
	vector<int> xIndices;

	// add collision sphers on x axis
	// for all collision spheres -> is within margin on y & z axes? if no -> remove
	// return remaining collision sphere indices

	Vec3 currentSphereGridIndices = getCurrentSphereGridPos(currentSphere);
	
	for (int i = currentSphereGridIndices[0] - 1; i <= currentSphereGridIndices[0] + 1; i++)
	{
		for (int j = 0; j < uniformGrids.gridX[i].size(); j++)
		{
			if (uniformGrids.gridX[i][j] != sphereIndex)
				xIndices.push_back(uniformGrids.gridX[i][j]);
		}
	}

	vector<int> xyIndices;
	for (int i = 0; i < xIndices.size(); i++)
	{
		bool exists = false;
		for (int j = currentSphereGridIndices[1] - 1; j <= currentSphereGridIndices[1] + 1; j++)
		{
			for (int h = 0; h < uniformGrids.gridY[j].size(); h++)
			{
				if (uniformGrids.gridY[j][h] == xIndices[i])
				{
					exists = true;
					break;
				}
			}
		}
		if (exists)
			xyIndices.push_back(xIndices[i]);
	}

	vector<int> finalIndices;
	for (int i = 0; i < xyIndices.size(); i++)
	{
		bool exists = false;
		for (int j = currentSphereGridIndices[2] - 1; j <= currentSphereGridIndices[2] + 1; j++)
		{
			for (int h = 0; h < uniformGrids.gridZ[j].size(); h++)
			{
				if (uniformGrids.gridZ[j][h] == xyIndices[i])
				{
					exists = true;
					break;
				}
			}
		}
		if (exists)
			finalIndices.push_back(xyIndices[i]);
	}

	return finalIndices;
}