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
	// Actor class to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ACreatures> CreatureActor;

	// List of creatures
	TArray<ACreatures*> CreaturesArray;

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

};