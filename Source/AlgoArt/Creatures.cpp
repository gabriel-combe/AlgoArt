// Fill out your copyright notice in the Description page of Project Settings.


#include "Creatures.h"

// Sets default values
ACreatures::ACreatures()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Default Scene");
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void ACreatures::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACreatures::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsCreated) return;

	for (int jointID = Joints.Num() - 1; jointID >= 0; jointID--) {
		FJoint joint = Joints[jointID];
		FQuat newRot = FMath::Lerp(FQuat(CreatureMesh[joint.Slave]->GetRelativeRotation()), FQuat(joint.TargetRotation), 0.01f);
		
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

	for (int bodyID = 0; bodyID < BodyCount; bodyID++) {
		TObjectPtr<UStaticMeshComponent> CurrentMesh = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), FName(FString::Printf(TEXT("Body%d"), bodyID)));
		
		CreatureMesh.Emplace(CurrentMesh);
		
		if (!CurrentMesh) continue;

		CurrentMesh->RegisterComponent();

		CurrentMesh->SetUseCCD(true);
		CurrentMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		CurrentMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		if (!CurrentIndex) {
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
			joint.LimitAngle = FVector(FMath::FRandRange(10.f, 90.f), FMath::FRandRange(10.f, 90.f), FMath::FRandRange(10.f, 90.f));

			Joints.Emplace(joint);
		}

		CurrentMesh->CreationMethod = EComponentCreationMethod::Instance;

		if (!Shape) continue;

		CurrentMesh->SetStaticMesh(Shape);

		PrevBodyID = CurrentIndex;
		CurrentIndex++;
		PrevBody = CurrentMesh;
	}

	IsCreated = true;
}

// Create a creature from an existing one
void ACreatures::CreateCopyCreature(ACreatures* source)
{
	DeactivateBrain();

	IsCreated = false;
	
	BodyCount = source->BodyCount;

	CreateCreature();

	for (int i = 0; i < Joints.Num(); i++) {
		Joints[i].LimitAngle = source->Joints[i].LimitAngle;
	}

	Brain = source->Brain;

	TargetPoint = source->TargetPoint;

	RandPeriod = source->RandPeriod;
}

// Create a creature randomly
void ACreatures::CreateRandomCreature()
{
	DeactivateBrain();

	IsCreated = false;

	BodyCount = FMath::RandRange(MinNbBody, MaxNbBody);

	CreateCreature();
}

// Setup the Brain
void ACreatures::SetupBrain()
{
	Brain.SetInputsNumber(5 + 3 * Joints.Num());
	Brain.SetOutputsNumber(3 * Joints.Num());
	TArray<int> HiddenLayers;
	HiddenLayers.Emplace(2 * (5 + 3 * Joints.Num()));
	HiddenLayers.Emplace(4 + 3 * Joints.Num());
	HiddenLayers.Emplace(int(0.5f * (5 + 3 * Joints.Num())));
	Brain.SetHiddenLayers(HiddenLayers);

	Brain.InitModel();
}

// Set the Target Point of the Creature
void ACreatures::SetTargetPoint(FVector targetpoint)
{
	TargetPoint = targetpoint;
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

// Use the Brain
void ACreatures::UseBrain()
{
	FMatrixNM Input = FMatrixNM(1, 5 + 3 * Joints.Num());
	float Distance = FVector::Dist(CreatureMesh[0]->GetRelativeLocation(), TargetPoint);
	Distance = FMath::Clamp(Distance, 0.f, 10000.f);
	Distance /= 10000.f;
	Input.SetValue(0, 0, Distance);
	Input.SetValue(0, 1, FMath::Cos(GetGameTimeSinceCreation() * RandPeriod));
	Input.SetValue(0, 2, FMath::Fmod(GetActorRotation().Roll, 360.f) / 360.f);
	Input.SetValue(0, 3, FMath::Fmod(GetActorRotation().Pitch, 360.f) / 360.f);
	Input.SetValue(0, 4, FMath::Fmod(GetActorRotation().Yaw, 360.f) / 360.f);
	for (int index = 5; index < Input.GetColumns(); index = index + 3) {
		Input.SetValue(0, index, FMath::Fmod(Joints[(index - 5) / 3].Rotation.Roll, 360.f) / 360.f);
		Input.SetValue(0, index + 1, FMath::Fmod(Joints[(index - 5) / 3].Rotation.Pitch, 360.f) / 360.f);
		Input.SetValue(0, index + 1, FMath::Fmod(Joints[(index - 5) / 3].Rotation.Yaw, 360.f) / 360.f);
	}

	FMatrixNM Output = Brain.RunForwardModel(Input);
	// UE_LOG(LogTemp, Warning, TEXT("Output Size %d x %d"), Output.GetRows(), Output.GetColumns());

	for (int index = 0; index < Output.GetColumns(); index = index + 3) {
		FRotator NewJointRot = FRotator(
			Output.GetValue(0, index) * Joints[index / 3].LimitAngle.X,
			Output.GetValue(0, index + 1) * Joints[index / 3].LimitAngle.Y,
			Output.GetValue(0, index + 2) * Joints[index / 3].LimitAngle.Z
		);

		/*FRotator NewJointRot = CreatureMesh[Joints[index / 3].Slave]->GetRelativeRotation() + FRotator(Output.GetValue(0, index) * 10.f, Output.GetValue(0, index + 1) * 10.f, Output.GetValue(0, index + 2) * 10.f);
		NewJointRot = FRotator(
			FMath::Clamp(NewJointRot.Roll, -Joints[index / 3].LimitAngle[0], Joints[index / 3].LimitAngle[0]),
			FMath::Clamp(NewJointRot.Pitch, -Joints[index / 3].LimitAngle[1], Joints[index / 3].LimitAngle[1]),
			FMath::Clamp(NewJointRot.Yaw, -Joints[index / 3].LimitAngle[2], Joints[index / 3].LimitAngle[2])
		);*/

		// UE_LOG(LogTemp, Warning, TEXT("New Joint %d Rot %f %f %f"), index / 3, NewJointRot.Roll, NewJointRot.Pitch, NewJointRot.Yaw);

		Joints[index / 3].TargetRotation = NewJointRot;
	}
}

// Activate the Brain
void ACreatures::ActivateBrain()
{
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &ACreatures::UseBrain);
	GetWorld()->GetTimerManager().SetTimer(RotTargetUpdate, Delegate, 0.5f, true, 0.f);
}

// Deactivate the Brain
void ACreatures::DeactivateBrain()
{
	GetWorld()->GetTimerManager().ClearTimer(RotTargetUpdate);
}

// Mutate the creature
void ACreatures::MutateCreature(float mutationrate)
{
	Brain.MutateNeuralNetwork(mutationrate);
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

