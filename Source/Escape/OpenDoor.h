// Copyright Joe Fatuch 2020

#pragma once

#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
		void OpenDoor(float DeltaTime);
	UFUNCTION()
		void CloseDoor(float DeltaTime);
	UFUNCTION()
		void FindAudioComponent();
	UFUNCTION()
		void FindPressurePlate();
	UFUNCTION()
		float TotalMassOfActors() const;

private:

	UPROPERTY(EditAnywhere, Category = "Audio")
		UAudioComponent* AudioComponent = nullptr;
	UPROPERTY(EditAnywhere, Category = "Interaction")
		ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere, Category = "Interaction")
		float TriggerMass = 50.f;

	UPROPERTY()
		float InitialYaw;
	UPROPERTY()
		float CurrentYaw;
	UPROPERTY()
		float TargetYaw;
	UPROPERTY()
		float DoorLastOpened = 0.f;
	UPROPERTY()
		float CurrentTime = 0.f;

	UPROPERTY(EditInstanceOnly, Category = "DoorRotation")
		float OpenAngle = 90.f;
	UPROPERTY(EditInstanceOnly, Category = "DoorRotation")
		float DoorCloseDelay = .5f;
	UPROPERTY(EditInstanceOnly, Category = "DoorRotation")
		float DoorOpenSpeed = 2.f;
	UPROPERTY(EditInstanceOnly, Category = "DoorRotation")
		float DoorCloseSpeed = 2.f;

	UPROPERTY()
		bool bDoorIsOpen = false;
};
