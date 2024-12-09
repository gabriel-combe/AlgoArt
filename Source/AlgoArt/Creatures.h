// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Creatures.generated.h"

USTRUCT(BlueprintType)
struct FJoint {
	GENERATED_BODY()

	// Joint Pos
	UPROPERTY(BlueprintReadWrite, Category = "Joint")
	FVector Position;

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
	//NeuralNetwork Brain;

	// Root Scene
	UPROPERTY(VisibleAnywhere, Category = "Creatures")
	TObjectPtr<USceneComponent> SceneComponent;

	// Array of meshes for the creature 
	UPROPERTY(VisibleAnywhere, Category = "Creatures")
	TArray<UStaticMeshComponent*> CreatureMesh;

	// Mesh to use for the creature
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> Shape = nullptr;

	// Minimum number of Body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	int MinNbBody = 2;

	// Maximum number of Body
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	int MaxNbBody = 5;

	// Minimum number of Limb
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	int MinNbLimbs = 2;

	// Maximum number of Limb
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Creatures", meta = (AllowPrivateAccess = "true"))
	int MaxNbLimbs = 4;

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

	// Array of joint
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Creatures")
	TArray<FJoint> Joints;

	// Create a creature randomly
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void CreateCreature();

	// Random Move
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creatures")
	void RandomMove();

	// Clear the creature
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Creature")
	void CreatureClear();

};
