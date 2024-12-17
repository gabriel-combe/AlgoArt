// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VegetationScatter.generated.h"

UCLASS()
class ALGOART_API AVegetationScatter : public AActor
{
	GENERATED_BODY()
	
private:
	// Root Scene
	UPROPERTY(VisibleAnywhere, Category = "Vegetation")
	TObjectPtr<USceneComponent> SceneComponent;

	// Array of meshes for the vegetation 
	UPROPERTY(VisibleAnywhere, Category = "Vegetation")
	TArray<UStaticMeshComponent*> Vegetations;

	// Tree array to scatter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vegetation", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMesh*> Trees;

	// Flower array to scatter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vegetation", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMesh*> Flowers;

	// Starting Radius for scattering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vegetation", meta = (AllowPrivateAccess = "true"))
	float StartRadius = 8500.f;

	// Ending Radius for scattering
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vegetation", meta = (AllowPrivateAccess = "true"))
	float EndRadius = 15000.f;

	// Tree scatter number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vegetation", meta = (AllowPrivateAccess = "true"))
	int TreeNumber = 10;

	// Flower scatter number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vegetation", meta = (AllowPrivateAccess = "true"))
	int FlowerNumber = 50;

	// Find Random Point in Donut shape circle
	FVector FindRandomPoint() const;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	AVegetationScatter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
