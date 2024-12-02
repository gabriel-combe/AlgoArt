// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class ALGOART_API ACell : public AActor
{
	GENERATED_BODY()
	
private:	
	// Mesh of the cell
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> CellMesh;

	// Material Interface of the cell mesh
	TObjectPtr<UMaterialInterface> Material;

	// Dynamic material for the cell mesh
	TObjectPtr<UMaterialInstanceDynamic> DynMaterial;

	// State of the cell
	bool State;

	// Previous state of the cell
	bool PreviousState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACell();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to set the state of the cell
	void SetInitState(bool state);

	// Called to change colour
	void ChangeColour(FVector colour);

	// Called to save the current state
	void SaveState();

	// Called to change the state
	void NewState(bool state);

	// Getter for previous state
	bool GetPreviousState() const { return PreviousState; };

	// Getter for current state
	bool GetState() const { return State; };
};
