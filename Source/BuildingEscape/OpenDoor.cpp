// © 2016 Daniel Garcia - All rights reserved

#include "BuildingEscape.h"
#include "OpenDoor.h"


AActor* OwnerObject;
APawn* PlayerPawn;

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	ActorThatOpens = PlayerPawn;
}

void UOpenDoor::OpenDoor()
{
	OwnerObject = GetOwner();

	FRotator NewRotation = FRotator(0.0f, -60.0f, 0.0f);

	OwnerObject->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the TriggerVolume every frame
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) //If Actor is in the pressure plate
	{
		OpenDoor();
	}
}

