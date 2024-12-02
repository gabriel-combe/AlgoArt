// Fill out your copyright notice in the Description page of Project Settings.


#include "CellularAutomata.h"

// Sets default values
ACellularAutomata::ACellularAutomata()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnCreation = true;

	GOLFlag = false;

	static ConstructorHelpers::FObjectFinder<UClass> CellBP(TEXT("Blueprint'/Game/Blueprints/MyCell.MyCell_C'"));
	if (CellBP.Succeeded())
		CellClass = CellBP.Object;
}

// Called when the game starts or when spawned
void ACellularAutomata::BeginPlay()
{
	Super::BeginPlay();
	CreateCells();

	for (int i = 0; i < 4; i++)
		GenerateCaveBreakOpen();

	for (int i = 0; i < 3; i++)
		GenerateCaveSimple();
}

// Called every frame
void ACellularAutomata::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!OnCreation && GOLFlag)
		GenerateGameOfLife();
}

// Called to create the grid of cells
void ACellularAutomata::CreateCells()
{
	OnCreation = true;

	ClearGrid();

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			ACell* Cell = GetWorld()->SpawnActor<ACell>(CellClass, FTransform(FRotator::ZeroRotator, FVector(x * SizeCell, y * SizeCell, SizeCell), FVector::OneVector));
			
			if (FMath::FRand() < InitProbaCell)
				Cell->SetInitState(1);
			else
				Cell->SetInitState(0);

			GridCell.Emplace(Cell);
		}
	}

	OnCreation = false;
}

// Called to generate the next state of the cells (Cave Simple)
void ACellularAutomata::GenerateCaveSimple()
{
	SaveGridCell();

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			ACell* Cell = GridCell[x + y*Width];

			int Neighbours = GetAliveNeighbours(x, y, 1);

			// Rules for cave generation
			if (Neighbours >= 5)
				Cell->NewState(1);
			else
				Cell->NewState(0);
		}
	}
}

// Called to generate the next state of the cells (Cave Break Open Space)
void ACellularAutomata::GenerateCaveBreakOpen()
{
	SaveGridCell();

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			ACell* Cell = GridCell[x + y * Width];

			int Neighbours1 = GetAliveNeighbours(x, y, 1);
			int Neighbours2 = GetAliveNeighbours(x, y, 2);

			// Rules for cave generation
			if (Neighbours1 >= 5 || Neighbours2 <= 1)
				Cell->NewState(1);
			else
				Cell->NewState(0);
		}
	}
}

// Called to generate the next state of the cells (Game Of Life)
void ACellularAutomata::GenerateGameOfLife()
{
	SaveGridCell();

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			ACell* Cell = GridCell[x + y * Width];

			int Neighbours = GetAliveNeighbours(x, y, 1) - Cell->GetState();

			// Rules for cave generation
			if (!Cell->GetState() && Neighbours == 3)
				Cell->NewState(1);
			else if (Cell->GetState() && (Neighbours < 2 || Neighbours > 3))
				Cell->NewState(0);
		}
	}
}

// Called to generate the next state of the cells (Mazectric)
void ACellularAutomata::GenerateMazectric()
{
	SaveGridCell();

	for (int y = 0; y < Height; y++) {
		for (int x = 0; x < Width; x++) {
			ACell* Cell = GridCell[x + y * Width];

			int Neighbours = GetAliveNeighbours(x, y, 1) - Cell->GetState();

			// Rules for cave generation
			if (!Cell->GetState() && Neighbours == 3)
				Cell->NewState(1);
			else if (Cell->GetState() && (Neighbours < 1 || Neighbours > 4))
				Cell->NewState(0);
		}
	}
}

// Called to clear the grid of cells
void ACellularAutomata::ClearGrid()
{
	for (ACell* cell : GridCell)
		cell->Destroy();

	GridCell.Empty();
}

// Called to get the number of living neighbour around k step of a cell
int ACellularAutomata::GetAliveNeighbours(int x, int y, int k)
{
	int Neighbours = 0;

	for (int i = -k; i <= k; i++)
		for (int j = -k; j <= k; j++)
			Neighbours += GridCell[(x+j+Width)%Width + ((y+i+Height)%Height)*Width]->GetPreviousState();

	return Neighbours;
}

// Called to save the grid
void ACellularAutomata::SaveGridCell()
{
	for (int y = 0; y < Height; y++)
		for (int x = 0; x < Width; x++)
			GridCell[x + y * Width]->SaveState();
}