// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NeuralNetworkModel.h"
#include "GameFramework/Actor.h"
#include "Creatures.generated.h"

USTRUCT(BlueprintType)
struct FJoint {
	GENERATED_BODY()

	// Joint Pos
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	FVector Position;

	// Joint Rotation
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	FRotator Rotation;

	// Joint Target Rotation
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	FRotator TargetRotation;

	// Joint Angle Limit
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	FVector LimitAngle;

	// Joint Master
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	int Master;

	// Joint Slave
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	int Slave;
};

UCLASS()
class ALGOART_API ACreatures : public AActor
{
	GENERATED_BODY()
	
private:	
	// Brain of the creature
	UNeuralNetworkModel Brain;

	// Timer Handle for target orientation update
	FTimerHandle RotTargetUpdate;

	// Target Point in the World
	FVector TargetPoint = FVector::Zero();

	// Number of Body Parts
	int BodyCount = 2;

	// Random Period
	float RandPeriod = FMath::FRandRange(1.f, 10.f);

	// Root Scene
	UPROPERTY(VisibleAnywhere, Category = "Creatures")
	TObjectPtr<USceneComponent> SceneComponent;

	// Mesh to use for the creature
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> Shape = nullptr;

	// Minimum number of Body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	int MinNbBody = 2;

	// Maximum number of Body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	int MaxNbBody = 8;

	// Maximum number of Limb
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	float SegmentSize = 100;

	// Is the creature created
	bool IsCreated = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ACreatures();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Array of meshes for the creature 
	UPROPERTY(VisibleAnywhere, Category = "Creatures")
	TArray<UStaticMeshComponent*> CreatureMesh;

	// Array of joint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creatures")
	TArray<FJoint> Joints;

	// Create a creature
	void CreateCreature();

	// Create a creature from an existing one
	void CreateCopyCreature(ACreatures* source);

	// Create a creature randomly
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void CreateRandomCreature();

	// Set the Target Point of the Creature
	UFUNCTION(BlueprintCallable, Category = "Creatures")
	void SetTargetPoint(FVector targetpoint);

	// Random Move
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void RandomMove();

	// Setup the Brain
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void SetupBrain();

	// Use the Brain
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void UseBrain();

	// Activate the Brain
	UFUNCTION(BlueprintCallable, Category = "Creatures")
	void ActivateBrain();

	// Deactivate the Brain
	UFUNCTION(BlueprintCallable, Category = "Creatures")
	void DeactivateBrain();

	// Mutate the creature
	UFUNCTION(BlueprintCallable, Category = "Creatures")
	void MutateCreature(float mutationrate);

	// Clear the creature
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void CreatureClear();

};
