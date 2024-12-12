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

	if (CreaturesArray.IsEmpty()) {
		for (int index = 0; index < NumberCreature; index++) {
			float angle = FMath::FRandRange(0.f, 2 * PI);
			ACreatures* creature = GetWorld()->SpawnActor<ACreatures>(CreatureActor, FTransform(FRotator(0.f, 0.f, FMath::RadiansToDegrees(angle + PI)), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
			creature->SetTargetPoint(TargetPoint);
			CreaturesArray.Emplace(creature);
		}
		return;
	}

	/*for (int index = 0; index < NumberCreature; index++) {
		float angle = FMath::FRandRange(0.f, 2 * PI);
		ACreatures* creature = GetWorld()->SpawnActor<ACreatures>(CreatureActor, FTransform(FRotator(0.f, 0.f, FMath::RadiansToDegrees(angle + 180.f)), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
		creature->SetTargetPoint(TargetPoint);
		CreaturesArray.Emplace(creature);
	}*/
}

