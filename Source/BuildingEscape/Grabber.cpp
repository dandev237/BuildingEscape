// © 2016 Daniel Garcia - All rights reserved

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

float UGrabber::GetReach() const {return Reach;}

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) {return;}
	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//Move the grabbed object
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}


void UGrabber::Grab() 
{
	//LINE TRACE - Reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	if (!PhysicsHandle) {return;}
	if (ActorHit)
	{
		//If we hit something, attach a physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab, //Grabbed component mesh
			NAME_None, //No bones
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true //Allow rotation
		); 
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
	
	if (!PhysicsHandle) {return;}
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	///Look for attached phyisics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle not found for %s"), *(GetOwner()->GetName()));
	}
}

void UGrabber::SetupInputComponent()
{
	///Look for attached input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		///Bind the input axis
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UInputComponent not found for %s"), *(GetOwner()->GetName()));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner()); //Ignore owner

																			   ///Raycast
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(OUT HitResult, GetLineTraceStart(), GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	///Log the Actor hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *(ActorHit->GetName()));
	}

	return HitResult;
}

const FVector UGrabber::GetLineTraceStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

const FVector UGrabber::GetLineTraceEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * GetReach();
}