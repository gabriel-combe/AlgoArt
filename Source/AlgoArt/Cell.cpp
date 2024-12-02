// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cell Mesh"));
	RootComponent = CellMesh;

	Material = CellMesh->GetMaterial(0);
	DynMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
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

	ChangeColour(FVector(State));
	SetActorScale3D(FVector(Scale.X, Scale.Y, 5 * State + 1));
}

// Called to change colour
void ACell::ChangeColour(FVector colour)
{
	DynMaterial->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(colour));
	CellMesh->SetMaterial(0, DynMaterial);
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
	ChangeColour(FVector(State));
}
