// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "OpenDoor.h"


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

	Owner = GetOwner();
	
	MainPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();

	// ...
	
}

void UOpenDoor::OpenDoor()
{


	Owner->SetActorRotation(FRotator(0.0f, DoorAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{

	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));

}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (PressurePlate->IsOverlappingActor(MainPlayer)) {

		OpenDoor();
		LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	}

	//Check how many seconds passed since door openned...
	float WorldTime = GetWorld()->GetTimeSeconds();
	
	if (WorldTime - LastOpenDoorTime > TimeToCloseDoor) {

		CloseDoor();
	
	}

	// ...
}

