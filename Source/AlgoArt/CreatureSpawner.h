// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creatures.h"
#include "GameFramework/Actor.h"
#include "CreatureSpawner.generated.h"

UCLASS()
class ALGOART_API ACreatureSpawner : public AActor
{
	GENERATED_BODY()
	
private:	
	// Timer Handle for Creature resample
	FTimerHandle TimerHandleResample;

	// Actor class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACreatures> CreatureActor;

	// Target Point for all the Creatures
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	FVector TargetPoint;

	//Number of Creatures
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	int NumberCreature = 10;

	// List of creatures
	TArray<ACreatures*> CreaturesArray;

	// List of Resampled Creature
	TArray<ACreatures*> ResampleCreature;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACreatureSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn Creatures
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void SpawnCreature();

	// Resample Creatures
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void CreatureResampling();

	// Mutation of the Creatures
	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void CreatureMutation();
};
