// Copyright Joe Fatuch 2020


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Identifies OUT parameters
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw = InitialYaw + OpenAngle;
	
	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > TriggerMass)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened	= GetWorld()->GetTimeSeconds();
	}
	else
	{
		CurrentTime = GetWorld()->GetTimeSeconds();
		if ( (CurrentTime - DoorLastOpened) > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, DoorOpenSpeed);
	FRotator DoorRotation(0.f, 0.f, 0.f);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if (!bDoorIsOpen)
	{
		AudioComponent->Play();
		bDoorIsOpen = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator DoorRotation(0.f, 0.f, 0.f);
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if (!AudioComponent) { return; }
	if (bDoorIsOpen)
	{
		AudioComponent->Play();
		bDoorIsOpen = false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	// Find all overlapping actors
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Add up their masses
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::FindPressurePlate()
{
	// Check for missing actor on component
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - Has the OpenDoor Component but does NOT have a PressurePlate set!"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s - does NOT have an Audio Component set!"), *GetOwner()->GetName());
	}
}