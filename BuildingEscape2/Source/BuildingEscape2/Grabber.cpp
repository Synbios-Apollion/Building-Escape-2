// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Default Pawn Reporting for duty"));
	
	PlayerController = GetWorld()->GetFirstPlayerController();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//FString ObjectName = GetOwner()->GetName();
	//GetOwner()->GetTransform().GetLocation().ToString();
	//FString ObjectPos = GetOwner()->GetActorLocation().ToString();
	//GetWorld()->GetFirstPlayerController()->GetPawn();
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	
	//TODO Log out to text
	//UE_LOG(LogTemp, Warning, TEXT("Positioned at %s, Looking at %s"), 
	//	*PlayerViewpointLocation.ToString(),
	//	*PlayerViewpointRotation.ToString())
	FVector LineTraceDirection = PlayerViewpointRotation.Vector() * Reach;
	FVector LineTraceEnd = PlayerViewpointLocation + LineTraceDirection;
	//Draw a red trace in the world to visualise
	DrawDebugLine(
		GetWorld(),
		PlayerViewpointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);

	// See what we hit

}

