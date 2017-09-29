// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

//	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName() );
	
}

void UOpenDoor::OpenDoor()
{
	if (!Owner)
		return;

	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	if (!Owner)
		return;

	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll TriggerVolume

	if (GetTotalMassOfActorsOnPlate() > 30.f) // TODO Make into a parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Check if its time to close the door;

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (auto& currentActor : OverlappingActors)
	{
		FString ObjectName = currentActor->GetName();

		UE_LOG(LogTemp, Warning, TEXT("%s"), *ObjectName);
		//TotalMass += currentActor->GetRootPrimitiveComponent()->GetMass();
		TotalMass += currentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}


	return TotalMass;
}

