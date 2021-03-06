// © 2016 Daniel Garcia - All rights reserved

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//Getter - Reach
	float GetReach() const;

	//Find attached Physics Handle component
	void FindPhysicsHandleComponent();

	//Sets up attached input component
	void SetupInputComponent();

	// Ray-cast and grab what's in reach
	void Grab();

	//Release something grabbed
	void Release();

	//Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns player position (beginning of reach line)
	const FVector GetLineTraceStart();

	//Returns current end of reach line
	const FVector GetLineTraceEnd();

		
private:
	//How far ahead of the player can we reach (cm.)
	UPROPERTY(EditAnywhere)
	float Reach = 100.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;
};
