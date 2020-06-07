// Copyright Joe Fatuch 2020

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Color.h"

// Identifies OUT parameters
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object we are holding
		UpdatePlayerViewpoint();
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

// Check for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does NOT have a PhysicsHandle Component set!"), *GetOwner()->GetName());
	}
}

// Check for Input Component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// Input Component is found
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does NOT have an Input Component set!"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	DrawDebugLine(GetWorld(), PlayerViewpoint.PlayerPosition, GetLineTraceEnd(), FColor(255, 0, 0), false, 0.f, 0, 3.f);

	FHitResult HitResult;
	// Trace parameters, set to ignore DefaultPawn collisions
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	// Ray-cast out to a certain distance (Reach)
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, PlayerViewpoint.PlayerPosition, GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	// Check the actor hit by the RayCast
	AActor* ActorHit = HitResult.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorHit->GetName());
	}

	return HitResult;
}

void UGrabber::Grab()
{
	UpdatePlayerViewpoint();
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// Attach Physics Handle to Hit Actor
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetLineTraceEnd());
	}
}

void UGrabber::Release()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::UpdatePlayerViewpoint()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpoint.PlayerPosition, OUT PlayerViewpoint.PlayerRotation);
}
// Return the players reach distance
FVector UGrabber::GetLineTraceEnd() const
{
	// Set the Ray Trace Length
	return PlayerViewpoint.PlayerPosition + PlayerViewpoint.PlayerRotation.Vector() * Reach;
}