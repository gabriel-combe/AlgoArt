// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create scene root component
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(DefaultSceneRoot);

	CellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell Mesh"));
	CellMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();

	if (!CellMesh) return;

	DynMaterial = UMaterialInstanceDynamic::Create(CellMesh->GetMaterial(0), CellMesh);
	CellMesh->SetMaterial(0, DynMaterial);
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to set the state of the cell
void ACell::SetInitState(bool state)
{
	State = state;
	PreviousState = State;

	FVector Scale = GetActorScale3D();

	ChangeColour(State);
	SetActorScale3D(FVector(Scale.X, Scale.Y, 5 * State + 1));
}

// Called to change colour
void ACell::ChangeColour(float colour)
{
	if (!DynMaterial || !CellMesh) return;

	DynMaterial->SetScalarParameterValue(FName("Color"), colour);
}

// Called to save the current state
void ACell::SaveState()
{
	PreviousState = State;
}

// Called to change the state
void ACell::NewState(bool state)
{
	State = state;

	FVector Scale = GetActorScale3D();

	SetActorScale3D(FVector(Scale.X, Scale.Y, 5*State + 1));
	ChangeColour(State);
}
