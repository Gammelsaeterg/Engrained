// Fill out your copyright notice in the Description page of Project Settings.


#include "Shroobs.h"

#include <chrono>

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Health.h"
#include "Mina.h"
#include "NavigationSystemTypes.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"


// Sets default values
AShroobs::AShroobs()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShroobs::BeginPlay()
{
	Super::BeginPlay();


}

void AShroobs::ActorState(float deltatime)
{
	switch (States) {
	case IDLE:			// Idle
		StateColor = colorIDLE;
		ActorIDLE(deltatime);
		EventHostile(false);
		break;
	case SHOCK:			// Shocked
		StateColor = colorSHOCK;
		ActorSHOCK(deltatime);
		EventHostile(false);
		break;
	case HOSTILE:		// Hostile
		StateColor = colorHOSTILE;
		ActorHOSTILE(deltatime);
		EventHostile(true);
		break;
	case AWAREOFPLAYER:	// Aware of player
		StateColor = colorAWAREOFPLAYER;
		ActorAWAREOFPLAYER(deltatime);
		EventHostile(false);
		break;
	case DEATH:			// Death
		UE_LOG(LogTemp, Warning, TEXT("%s has died"), *GetName());
		break;
	default:
		break;
	}
}

void AShroobs::ActorIDLE(float deltatime)
{
	CurrentLocation = GetActorLocation();
	CurrentLocation += GetActorForwardVector() * Speed * deltatime;
	SetActorLocation(CurrentLocation);

	

	//UE_LOG(LogTemp, Display, TEXT("Rotate: %s"), (bRotateBack ? TEXT("true") : TEXT("false")));
	/* Movement across platform */
	RayTraceSeconds += deltatime;
	if (RayTraceSeconds > RayTraceTiming) {
		if (RayTraceActive) {
			MoveAreaCheck(RayTraceLength);

			/* Draws debugline matching the raytrace */
			if (bDebugLine) {
				DrawDebugLine(
					GetWorld(),
					this->GetActorLocation(),
					this->GetActorLocation() - FVector(0, 0, RayTraceLength),
					FColor(255, 0, 0),
					false,
					.2f,
					0,
					1.f
				);
			}
		}
		RayTraceSeconds = 0;
	}
	/* Gives a random location related to the shroobs area of movement */
	

	if (bRotateBack) {
		//FVector MoveToVector;
		//if (!FindRandomArea) {
		//	MoveToVector = MoveAreaVector + FVector(FMath::RandRange(
		//		-RotationPointRandomRange, RotationPointRandomRange),
		//		FMath::RandRange(-RotationPointRandomRange, RotationPointRandomRange), 0);
		//	FindRandomArea++;
		//}
		RotateToVector(MoveAreaVector, 15.f);
		//RotateToVector();
	}

	if (bDebugPlayerDetection)
		DetectPlayer(deltatime);
}

void AShroobs::ActorSHOCK(float deltatime)
{
	RotateToVector(PlayerLocation, Rotation_Z);

	//UE_LOG(LogTemp, Display, TEXT("Shocked: %f"), TimeShocked);
	TimeShocked += deltatime;
	if (TimeShocked > ShockTimer) {
		//UE_LOG(LogTemp, Display, TEXT("%s is HOSTILE!"), *GetName());
		TimeShocked = 0;
		States = HOSTILE;
	}
}

void AShroobs::ActorHOSTILE(float deltatime)
{
	/* If player is not within reach
		Run timer
		if timer exceeds x. Go to state AWAREOFPLAYER */

	RotateToVector(PlayerLocation, Rotation_Z);

	if (OtherActorWithinReach(PlayerLocation, HostileReach)) {
		DrawDebugLineBetweenActors(PlayerLocation, StateColor);
		TimeHostile = 0;
	}
	else {
		//UE_LOG(LogTemp, Display, TEXT("TimeHostile %f"), TimeHostile);
		TimeHostile += deltatime;
	}
	if (TimeHostile > HostileTimer) {
		//UE_LOG(LogTemp, Display, TEXT("Player left %s's reach"), *GetName());
		//UE_LOG(LogTemp, Display, TEXT("%s is now AWARE OF PLAYER"), *GetName());
		States = AWAREOFPLAYER;
		TimeHostile = 0;
		TimeAware = 0;
		EventHostile(false);
	}
}

void AShroobs::ActorAWAREOFPLAYER(float deltatime)
{
	TimeAwareMovement += deltatime;

	RotateToVector(PlayerLocation, Rotation_Z);

	if (TimeAwareMovement > AwareMovementTimer) {
		/* For øyeblikket er det en super simple fram og tilbake 
			i forhold til Actor Right Vector */
		Length = Length * -1;
		//UE_LOG(LogTemp, Error, TEXT("Length: %f"), Length);

		TimeAwareMovement = 0;
		UE_LOG(LogTemp, Warning, TEXT("AWARE: changing direction"));
	}
	// Add local offset
	AddActorLocalOffset(GetActorRightVector() * (Length * AwareMovementSpeed));

	

	if (!OtherActorWithinReach(PlayerLocation, HostileReach)) {
		DrawDebugLineBetweenActors(PlayerLocation, StateColor);
		//UE_LOG(LogTemp, Display, TEXT("TimeAware %f"), TimeAware);
		TimeAware += deltatime;
		if (TimeHostile > 0)
			TimeHostile -= deltatime;
	}
	else {
		DrawDebugLineBetweenActors(PlayerLocation, colorHOSTILE);
		//UE_LOG(LogTemp, Display, TEXT("TimeHostile %f"), TimeHostile);
		TimeHostile += deltatime;
		
		if (TimeAware > 0)
			TimeAware -= deltatime;
	}
	if (TimeHostile > HostileTimer/2) {
		//UE_LOG(LogTemp, Display, TEXT("%s is HOSTILE!"), *GetName());
		States = HOSTILE;
		TimeAware = 0;
		EventHostile(true);
	}
	if (TimeAware > AwareTimer) {
		//UE_LOG(LogTemp, Display, TEXT("%s is IDLE again"), *GetName());
		States = IDLE;
		TimeHostile = 0;
	}
}

// Called every frame
void AShroobs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Seconds += DeltaTime;
	
	ActorState(DeltaTime);


}

void AShroobs::ImHit()
{
	Destroy();
}




