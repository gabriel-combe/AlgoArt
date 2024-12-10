// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures.h"

// Sets default values
ACreatures::ACreatures()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*SceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene");
	RootComponent = SceneComponent;*/
}

// Called when the game starts or when spawned
void ACreatures::BeginPlay()
{
	Super::BeginPlay();
	
	IsCreated = false;
	
	CreateCreature();

	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACreatures::RandomMove);
	GetWorld()->GetTimerManager().SetTimer(RotTargetUpdate, Delegate, 0.5f, true, 0.f);
}

// Called every frame
void ACreatures::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsCreated) return;

	for (int jointID = Joints.Num() - 1; jointID >= 0; jointID--) {
		FJoint joint = Joints[jointID];
		FQuat newRot = FMath::Lerp(FQuat(CreatureMesh[joint.Slave]->GetRelativeRotation()), FQuat(joint.TargetRotation), 0.05f);
		
		CreatureMesh[joint.Slave]->SetRelativeRotation(newRot, true);
	}
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
		
		if (!CurrentMesh) continue;

		CurrentMesh->RegisterComponent();

		CurrentMesh->SetUseCCD(true);
		CurrentMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		CurrentMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		if (!CurrentIndex) {
			//RootComponent = CurrentMesh;
			CreatureMesh[CurrentIndex]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			CurrentMesh->SetSimulatePhysics(true);
		}
		else {
			CurrentMesh->AddRelativeLocation(FVector(-SegmentSize, 0, 0));
			CurrentMesh->SetRelativeScale3D(FVector(1, PrevBody->GetRelativeScale3D()[1] * 0.98f, PrevBody->GetRelativeScale3D()[2] * 0.98f));
			CurrentMesh->AttachToComponent(PrevBody, FAttachmentTransformRules::KeepRelativeTransform);

			FJoint joint;
			joint.Master = PrevBodyID;
			joint.Slave = CurrentIndex;
			joint.Position = CurrentMesh->GetRelativeLocation();
			joint.Rotation = CurrentMesh->GetRelativeRotation();
			joint.TargetRotation = CurrentMesh->GetRelativeRotation();
			joint.LimitAngle = FVector(80, 25, 25);

			Joints.Emplace(joint);
		}

		CurrentMesh->CreationMethod = EComponentCreationMethod::Instance;

		if (!Shape) continue;

		CurrentMesh->SetStaticMesh(Shape);

		CurrentIndex++;
		PrevBody = CurrentMesh;
	}

	IsCreated = true;
}

// Random Move
void ACreatures::RandomMove()
{
	for (int jointID = 0; jointID < Joints.Num(); jointID++) {
		FJoint joint = Joints[jointID];
		float angleX = FMath::FRandRange(-10.f, 10.f);
		float angleY = FMath::FRandRange(-10.f, 10.f);
		float angleZ = FMath::FRandRange(-10.f, 10.f);

		FRotator angle = CreatureMesh[joint.Slave]->GetRelativeRotation() + FRotator(angleX, angleY, angleZ);
		angle = FRotator(
			FMath::Clamp(angle.Roll, -joint.LimitAngle[0], joint.LimitAngle[0]),
			FMath::Clamp(angle.Pitch, -joint.LimitAngle[1], joint.LimitAngle[1]),
			FMath::Clamp(angle.Yaw, -joint.LimitAngle[2], joint.LimitAngle[2])
		);

		Joints[jointID].TargetRotation = angle;
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

