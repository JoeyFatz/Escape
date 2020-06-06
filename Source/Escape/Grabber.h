// Copyright Joe Fatuch 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	UPROPERTY(EditAnywhere, Category = "Interaction")
		float Reach = 100.f;

	UFUNCTION()
		void Grab();
	UFUNCTION()
		void Release();
	UFUNCTION()
		void FindPhysicsHandle();
	UFUNCTION()
		void SetupInputComponent();
	UFUNCTION() // Return first actor within reach with a physics body
	FHitResult 	GetFirstPhysicsBodyInReach() const;
};
