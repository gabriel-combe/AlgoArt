// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureSpawner.h"

// Sets default values
ACreatureSpawner::ACreatureSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACreatureSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnCreature();	
}

// Called every frame
void ACreatureSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Spawn Creatures
void ACreatureSpawner::SpawnCreature()
{
	if (!CreatureActor) return;

	for (int index = 0; index < 5; index++) {
		ACreatures* creature = GetWorld()->SpawnActor<ACreatures>(CreatureActor, FTransform(FRotator(FMath::FRandRange(0.f, 360.f)), FVector(FMath::FRandRange(0.f, 10000.f), FMath::FRandRange(0.f, 10000.f), 500.f), FVector::OneVector));
		CreaturesArray.Emplace(creature);
	}
}

