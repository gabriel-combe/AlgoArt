// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cell.h"
#include "GameFramework/Actor.h"
#include "CellularAutomata.generated.h"

UCLASS()
class ALGOART_API ACellularAutomata : public AActor
{
	GENERATED_BODY()
	
private:	
	// Class of the cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACell> CellClass;

	// Check if the grid is currently being generated
	bool OnCreation;

	// Activate the game of life
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata", meta = (AllowPrivateAccess = "true"))
	bool GOLFlag;

	// Init Proba for the cells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata", meta = (AllowPrivateAccess = "true"))
	float InitProbaCell = 0.5f;

	// Width of the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata", meta = (AllowPrivateAccess = "true"))
	int Width = 100;

	// Height of the grid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata", meta = (AllowPrivateAccess = "true"))
	int Height = 100;

	// Size of the cells
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cellular Automata", meta = (AllowPrivateAccess = "true"))
	int SizeCell = 100;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Grid of cells
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cellular Automata")
	TArray<ACell*> GridCell;

public:	
	// Sets default values for this actor's properties
	ACellularAutomata();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to create the grid of cells
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cellular Automata")
	void CreateCells();
	
	// Called to generate the next state of the cells (Cave Simple)
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cellular Automata")
	void GenerateCaveSimple();

	// Called to generate the next state of the cells (Cave Break Open Space)
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cellular Automata")
	void GenerateCaveBreakOpen();

	// Called to generate the next state of the cells (Game Of Life)
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cellular Automata")
	void GenerateGameOfLife();

	// Called to generate the next state of the cells (Mazectric)
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cellular Automata")
	void GenerateMazectric();

	// Called to clear the grid of cells
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Cellular Automata")
	void ClearGrid();

	// Called to get the number of living neighbour around k step of a cell
	UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
	int GetAliveNeighbours(int x, int y, int k);

	// Called to save the grid
	UFUNCTION(BlueprintCallable, Category = "Cellular Automata")
	void SaveGridCell();
};
