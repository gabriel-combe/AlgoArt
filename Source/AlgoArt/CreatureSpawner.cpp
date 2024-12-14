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

	ResamplingNumber = int(NumberCreature * ResamplingRatio);

	SpawnCreature();

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACreatureSpawner::CreatureResampling);
	GetWorld()->GetTimerManager().SetTimer(TimerHandleResampling, Delegate, 10.f, true, -10.f);
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
			ACreatures* creature = GetWorld()->SpawnActor<ACreatures>(CreatureActor, FTransform(FRotator(0.f, FMath::RadiansToDegrees(angle + PI), 0.f), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
			creature->CreateRandomCreature();
			creature->SetupBrain();
			creature->ActivateBrain();
			CreaturesArray.Emplace(creature);
		}
		return;
	}

	for (int index = 0; index < ResamplingNumber; index++) {
		float angle = FMath::FRandRange(0.f, 2 * PI);
		CreaturesArray[index]->CreatureMesh[0]->ResetRelativeTransform();
		CreaturesArray[index]->SetActorRelativeTransform(FTransform(FRotator(0.f, FMath::RadiansToDegrees(angle + PI), 0.f), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
	}

	for (int index = ResamplingNumber; index < NumberCreature; index++) {
		float angle = FMath::FRandRange(0.f, 2 * PI);
		CreaturesArray[index]->CreatureMesh[0]->ResetRelativeTransform();
		CreaturesArray[index]->CreateCopyCreature(CreaturesArray[index % ResamplingNumber]);
		CreaturesArray[index]->ActivateBrain();
		CreaturesArray[index]->SetActorRelativeTransform(FTransform(FRotator(0.f, FMath::RadiansToDegrees(angle + PI), 0.f), FVector(FMath::Cos(angle) * 8000.f, FMath::Sin(angle) * 8000.f, 200.f), FVector::OneVector));
	}
}

// Resample Creatures
void ACreatureSpawner::CreatureResampling()
{
	CreaturesArray.Sort([](const ACreatures& ip1, const ACreatures& ip2) {
		return ip1.CreatureMesh[0]->GetRelativeLocation().Length() > ip2.CreatureMesh[0]->GetRelativeLocation().Length();
	});

	SpawnCreature();

	CreatureMutation();
}

// Mutate the creatures
void ACreatureSpawner::CreatureMutation()
{
	for (int index = ResamplingNumber; index < NumberCreature; index++) {
		CreaturesArray[index]->MutateCreature(MutationRate);
	}
}

