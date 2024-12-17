// Fill out your copyright notice in the Description page of Project Settings.


#include "VegetationScatter.h"

// Sets default values
AVegetationScatter::AVegetationScatter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene");
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void AVegetationScatter::BeginPlay()
{
	Super::BeginPlay();

	Vegetations.Empty();

	if (Trees.IsEmpty()) return;
	if (Flowers.IsEmpty()) return;

	for (int i = 0; i < TreeNumber; i++) {
		TObjectPtr<UStaticMeshComponent> CurrentMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(FString::Printf(TEXT("Tree%d"), i)));

		if (!CurrentMesh) continue;

		CurrentMesh->RegisterComponent();

		CurrentMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		CurrentMesh->SetRelativeTransform(FTransform(FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f), FindRandomPoint(), FVector::OneVector));

		CurrentMesh->CreationMethod = EComponentCreationMethod::Instance;

		CurrentMesh->SetStaticMesh(Trees[FMath::RandRange(0, Trees.Num() - 1)]);
	}

	for (int i = 0; i < FlowerNumber; i++) {
		TObjectPtr<UStaticMeshComponent> CurrentMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(FString::Printf(TEXT("Flower%d"), i)));

		if (!CurrentMesh) continue;

		CurrentMesh->RegisterComponent();

		CurrentMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		CurrentMesh->SetRelativeTransform(FTransform(FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f), FindRandomPoint(), FVector::OneVector));

		CurrentMesh->CreationMethod = EComponentCreationMethod::Instance;

		CurrentMesh->SetStaticMesh(Flowers[FMath::RandRange(0, Flowers.Num() - 1)]);
	}
	
}

// Called every frame
void AVegetationScatter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Find Random Point in Donut shape circle
FVector AVegetationScatter::FindRandomPoint() const
{
	FVector Location;
	bool IsValid = false;

	while (!IsValid) {
		Location = FVector(FMath::FRandRange(-EndRadius, EndRadius), FMath::FRandRange(-EndRadius, EndRadius), 0.f);

		bool OutInnerCircle = Location.X * Location.X + Location.Y * Location.Y >= StartRadius * StartRadius;
		bool InOuterCircle = Location.X * Location.X + Location.Y * Location.Y <= EndRadius * EndRadius;

		if (OutInnerCircle && InOuterCircle) IsValid = true;
	}

	return Location;
}