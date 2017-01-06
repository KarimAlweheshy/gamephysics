#include "UniformGrid.h"

UniformGrid::UniformGrid(int i_width, float f_cellWidth)
{
	m_iSize = i_width;
	m_fCellSize = f_cellWidth;
}

vector<vector<int>> UniformGrid::checkGrid(SphereSystem * system, gridPlane plane)
{
	fillGrid(plane);
	// for all spheres:
	//	1. add index to row & get gridPosition <x,y>
	//  for all surrounding squares:
	//		a. check square for members
	//		IF !empty_square
	//			add indeces of all square members to row

	vector<vector<int>> indexMatrix;

	for (int currentIndex = 0; currentIndex < system->spheres.size(); currentIndex++)
	{
		// add index to row
		vector<int> row;
		row.push_back(currentIndex);

		vector<int> gridPos = getCurrentSphereGridPos(&system->spheres[currentIndex], plane);

		// iterate through all surrounding squares
		for (int x = gridPos[0] - 1; x <= gridPos[0] + 1; x++)
		{
			for (int y = gridPos[1] - 1; y <= gridPos[1] + 1; y++)
			{
				// add all indices in surrounding cell to row
				for (int collisionIndex = 0; collisionIndex < grid[x][y].size(); collisionIndex++)
				{
					// skip collision with own sphere
					if (grid[x][y][collisionIndex] != currentIndex);
						row.push_back(grid[x][y][collisionIndex]);
				}
			}
		}

		indexMatrix.push_back(row);
	}

	return indexMatrix;
}

/* TODO */
vector<int> UniformGrid::getCurrentSphereGridPos(Sphere * sphere, gridPlane plane)
{
	vector<int> pos;

	switch (plane)
	{
	case gridPlane::XY_PLANE:
		break;

	case gridPlane::XZ_PLANE:
		break;

	case gridPlane::YZ_PLANE:
		break;
	}

	return pos;
}

/* TODO */
void UniformGrid::fillGrid(gridPlane plane)
{

}