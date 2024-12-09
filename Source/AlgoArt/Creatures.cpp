// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures.h"

// Sets default values
ACreatures::ACreatures()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene");
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void ACreatures::BeginPlay()
{
	Super::BeginPlay();
	
	IsCreated = false;
	
	CreateCreature();
}

// Called every frame
void ACreatures::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsCreated) return;

	RandomMove();
}

// Create a creature randomly
void ACreatures::CreateCreature()
{
	CreatureClear();

	int CurrentIndex = 0;
	int PrevBodyID = 0;
	TObjectPtr<UStaticMeshComponent> PrevBody;
	int BodyCount = FMath::RandRange(MinNbBody, MaxNbBody);

	for (int bodyID = 0; bodyID < BodyCount; bodyID++) {
		PrevBodyID = CurrentIndex;
		TObjectPtr<UStaticMeshComponent> CurrentMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(FString::Printf(TEXT("Body%d"), bodyID)));
		
		CreatureMesh.Emplace(CurrentMesh);
		
		if (!CreatureMesh[CurrentIndex]) continue;

		CreatureMesh[CurrentIndex]->RegisterComponent();

		if (!CurrentIndex) {
			CreatureMesh[CurrentIndex]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CreatureMesh[CurrentIndex]->SetSimulatePhysics(true);
		}
		else {
			CreatureMesh[CurrentIndex]->AddRelativeLocation(FVector(-100, 0, 0));
			CreatureMesh[CurrentIndex]->SetRelativeScale3D(FVector(1, PrevBody->GetRelativeScale3D()[1] * 0.98f, PrevBody->GetRelativeScale3D()[2] * 0.98f));
			CreatureMesh[CurrentIndex]->AttachToComponent(PrevBody, FAttachmentTransformRules::KeepRelativeTransform);

			FJoint joint;
			joint.Master = PrevBodyID;
			joint.Slave = CurrentIndex;
			joint.Position = CreatureMesh[CurrentIndex]->GetRelativeLocation();
			joint.LimitAngle = FVector(80, 25, 25);

			Joints.Emplace(joint);
		}

		CreatureMesh[CurrentIndex]->CreationMethod = EComponentCreationMethod::UserConstructionScript;

		if (!Shape) continue;

		CreatureMesh[CurrentIndex]->SetStaticMesh(Shape);

		int LimbCount = FMath::RandRange(MinNbLimbs, MaxNbLimbs);
		
		CurrentIndex++;
		PrevBody = CurrentMesh;
		/*for (int limbID = 0; limbID < LimbCount; limbID++) {
			CreatureMesh[CurrentIndex] = CreateDefaultSubobject<UStaticMeshComponent>("Limb");
			CreatureMesh[CurrentIndex]->SetStaticMesh(Shape);
			CreatureMesh[CurrentIndex]->SetRelativeScale3D(
				FVector(
					FMath::FRandRange(0.1f, 1.f),
					FMath::FRandRange(0.1f, 1.f),
					FMath::FRandRange(0.1f, 1.f)
				));

			CreatureMesh[CurrentIndex]->SetupAttachment(CreatureMesh[PrevBodyID]);

			CurrentIndex++;
		}*/
	}

	IsCreated = true;
}

// Random Move
void ACreatures::RandomMove()
{
	for (int jointID = 0; jointID < Joints.Num(); jointID++) {
		FJoint joint = Joints[jointID];
		float angleX = FMath::FRandRange(-0.5f, 0.5f);
		float angleY = FMath::FRandRange(-0.5f, 0.5f);
		float angleZ = FMath::FRandRange(-0.5f, 0.5f);

		FRotator angle = CreatureMesh[joint.Slave]->GetRelativeRotation() + FRotator(angleX, angleY, angleZ);
		angle = FRotator(
			FMath::Clamp(angle.Roll, -joint.LimitAngle[0], joint.LimitAngle[0]),
			FMath::Clamp(angle.Pitch, -joint.LimitAngle[1], joint.LimitAngle[1]),
			FMath::Clamp(angle.Yaw, -joint.LimitAngle[2], joint.LimitAngle[2])
		);

		CreatureMesh[joint.Slave]->SetRelativeRotation(FQuat(angle));
	}
}

// Clear the creature
void ACreatures::CreatureClear()
{
	for (UStaticMeshComponent* meshComp : CreatureMesh) {
		meshComp->DestroyComponent();
	}

	CreatureMesh.Empty();

	Joints.Empty();
}

