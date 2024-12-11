// Fill out your copyright notice in the Description page of Project Settings.


#include "CreatureSpawner.h"

// Sets default values
ACreatureSpawner::ACreatureSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ResampleCreature.SetNum(int(0.2 * NumberCreature));
}

// Called when the game starts or when spawned
void ACreatureSpawner::BeginPlay()
{
	Super::BeginPlay();

	ResampleCreature.Empty();

	SpawnCreature();	

	ResampleCreature.SetNum(int(0.2 * NumberCreature));

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACreatureSpawner::CreatureResampling);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleResample, Delegate, 10.f, true);
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

	if (ResampleCreature.IsEmpty()) {
		for (int index = 0; index < NumberCreature; index++) {
			float angle = FMath::FRandRange(0.f, 2 * PI);
			ACreatures* creature = GetWorld()->SpawnActor<ACreatures>(CreatureActor, FTransform(FRotator(0.f, 0.f, FMath::RadiansToDegrees(angle + 180.f)), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
			creature->SetTargetPoint(TargetPoint);
			CreaturesArray.Emplace(creature);
		}
		return;
	}

	for (int index = 0; index < NumberCreature; index++) {
		float angle = FMath::FRandRange(0.f, 2 * PI);
		ACreatures* creature = GetWorld()->SpawnActor<ACreatures>(CreatureActor, FTransform(FRotator(0.f, 0.f, FMath::RadiansToDegrees(angle + 180.f)), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
		creature->SetTargetPoint(TargetPoint);
		creature->SetBrain(ResampleCreature[index % ResampleCreature.Num()]->GetBrain());
		CreaturesArray.Emplace(creature);
	}

	ResampleCreature.Empty();
	ResampleCreature.SetNum(int(0.2 * NumberCreature));
}

// Resample Creatures
void ACreatureSpawner::CreatureResampling()
{
	CreaturesArray.Sort([](const ACreatures& ip1, const ACreatures& ip2) {
		return ip1.CreatureMesh[0]->GetRelativeLocation().Length() < ip2.CreatureMesh[0]->GetRelativeLocation().Length();
	});

	for (int i = 0; i < ResampleCreature.Num(); i++) {
		ResampleCreature[i] = CreaturesArray[i];
	}

	SpawnCreature();

	CreatureMutation();
}

// Mutation of the Creatures
void ACreatureSpawner::CreatureMutation()
{
	for (ACreatures* creature : CreaturesArray) {
		creature->BrainMutation(0.01f);
	}
}

