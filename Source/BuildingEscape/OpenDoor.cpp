// © 2016 Daniel Garcia - All rights reserved

#include "BuildingEscape.h"
#include "OpenDoor.h"

#define OUT

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

	OwnerObject = GetOwner();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	OwnerObject->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	OwnerObject->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Poll the TriggerVolume every frame
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate pointer is null!!!"));
		return 0.f;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Get the total mass
	for (const AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->GetRootPrimitiveComponent()->GetMass();
	}

	//UE_LOG(LogTemp, Warning, TEXT("Plate mass: %f"), TotalMass);
	return TotalMass;
}