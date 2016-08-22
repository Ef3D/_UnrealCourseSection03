// Fill out your copyright notice in the Description page of Project Settings.

#include "EscapeRoom.h"
#include "Grabber.h"

#define OUT
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	
	PrimaryComponentTick.bCanEverTick = true;

	

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT(" Yay we have the Physics Handle Component %s "), *GetOwner()->GetName());

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Missing Physics Handle Component %s "), *GetOwner()->GetName());
	}

	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT(" Good, InputComponent is On!!! "));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Release", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT(" Missing InputComponent %s "), *GetOwner()->GetName());
	}

	MyController = GetWorld() -> GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("NewPawn is Active and in the Game!!!"));
	// ...
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	
	MyController->GetPlayerViewPoint(OUT ViewLocation, OUT ViewRotation);

	FVector ViewLineEnd = (ViewLocation) + ViewRotation.Vector() * Reach;
	
	if (PhysicsHandle->GrabbedComponent) 
	{
		PhysicsHandle->SetTargetLocation(ViewLineEnd);
	}
	// ...
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT(" GrabPressed !!! "));

	FVector ViewLineEnd = (ViewLocation)+ViewRotation.Vector() * Reach;
	
	auto HitResult = LineTracerHit(ViewLocation, ViewLineEnd);
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	if (ActorHit) 
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ActorHit->GetActorLocation(),
			false
		);

		UE_LOG(LogTemp, Warning, TEXT("The Raycast has hit this Actor %s"), *(ActorHit->GetName()));

	}
}

void UGrabber::Release()
{
	
	UE_LOG(LogTemp, Warning, TEXT(" GrabReleased !!! "));

	PhysicsHandle->ReleaseComponent();

}

FHitResult UGrabber::LineTracerHit(FVector Start, FVector End) {

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(

		Hit,
		Start,
		End,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);

	return  Hit;
}
