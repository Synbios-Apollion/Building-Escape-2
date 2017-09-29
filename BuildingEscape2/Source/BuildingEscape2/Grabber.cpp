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
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = GetWorld()->GetFirstPlayerController();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Pressed"));

	///Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	// attach physics handle
	if (ActorHit != nullptr)
	{

		if (!PhysicsHandle)
			return;

		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
			);
	}
}

void UGrabber::Release()
{

	if (!PhysicsHandle)
		return;

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Component not found for %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	///Look for attached input component (only appears at runtime)
	if (InputComponent != nullptr)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s input component not found"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Setup Query Params
	FCollisionQueryParams TraceParameters(FName(), false, GetOwner());

	/// Line-trace (RAY CAST) out to reach distance
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	//// See what we hit
	//AActor* ActorHit = Hit.GetActor();
	//if (ActorHit)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Looking at %s"), *(ActorHit->GetName()));
	//}

	return Hit;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle)
		return;
	//if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}

}


FVector UGrabber::GetReachLineStart()
{
	//move the object that we're holding
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	return PlayerViewpointLocation;

}

FVector UGrabber::GetReachLineEnd()
{
	//move the object that we're holding
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	PlayerController->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);


	return PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;
}