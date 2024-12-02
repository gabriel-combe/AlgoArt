#pragma once

#include "CoreMinimal.h"
#include "Cell.h"
#include "Rules.generated.h"

// Called to get the number of living neighbour around k step of a cell
int GetAliveNeighbours(int x, int y, int k, TArray<ACell*> GridCell, int Width, int Height)
{
	int Neighbours = 0;

	for (int i = -k; i <= k; i++)
		for (int j = -k; j <= k; j++)
			Neighbours += GridCell[(x + j + Width) % Width + ((y + i + Height) % Height) * Width]->GetPreviousState();

	return Neighbours;
}

bool CaveSimple(int x, int y, TArray<ACell*> GridCell, int Width, int Height)
{
	int Neighbours = GetAliveNeighbours(x, y, 1, GridCell, Width, Height);

	return Neighbours >= 5;
}

bool CaveBreakOpen(int x, int y, TArray<ACell*> GridCell, int Width, int Height)
{
	int Neighbours1 = GetAliveNeighbours(x, y, 1, GridCell, Width, Height);
	int Neighbours2 = GetAliveNeighbours(x, y, 2, GridCell, Width, Height);

	return Neighbours1 >= 5 || Neighbours2 <= 1;
}

bool GameOfLife(int Neighbours)
{

}