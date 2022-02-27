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
		break;
	case SHOCK:			// Shocked
		StateColor = colorSHOCK;
		ActorSHOCK(deltatime);
		break;
	case HOSTILE:		// Hostile
		StateColor = colorHOSTILE;
		ActorHOSTILE(deltatime);
		break;
	case AWAREOFPLAYER:	// Aware of player
		StateColor = colorAWAREOFPLAYER;
		ActorAWAREOFPLAYER(deltatime);
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
					2.f,
					0,
					1.f
				);
			}
		}
		RayTraceSeconds = 0;
	}
	if (bRotateBack) {
		RotateToVector();
	}

	if (bDebugPlayerDetection)
		DetectPlayer(deltatime);
}

void AShroobs::ActorSHOCK(float deltatime)
{
	UE_LOG(LogTemp, Display, TEXT("Shocked: %f"), TimeShocked);
	TimeShocked += deltatime;
	if (TimeShocked > ShockTimer) {
		UE_LOG(LogTemp, Display, TEXT("%s is HOSTILE!"), *GetName());
		TimeShocked = 0;
		States = HOSTILE;
	}
}

void AShroobs::ActorHOSTILE(float deltatime)
{
	/* If player is not within reach
		Run timer
		if timer exceeds x. Go to state AWAREOFPLAYER */

	if (OtherActorWithinReach(PlayerLocation, HostileReach)) {
		DrawDebugLineBetweenActors(PlayerLocation, StateColor);
		TimeHostile = 0;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("TimeHostile %f"), TimeHostile);
		TimeHostile += deltatime;
	}
	if (TimeHostile > HostileTimer) {
		UE_LOG(LogTemp, Display, TEXT("Player left %s's reach"), *GetName());
		UE_LOG(LogTemp, Display, TEXT("%s is now AWARE OF PLAYER"), *GetName());
		States = AWAREOFPLAYER;
		TimeAware = 0;
	}
}

void AShroobs::ActorAWAREOFPLAYER(float deltatime)
{
	if (!OtherActorWithinReach(PlayerLocation, HostileReach)) {
		DrawDebugLineBetweenActors(PlayerLocation, StateColor);
		UE_LOG(LogTemp, Display, TEXT("TimeAware %f"), TimeAware);
		TimeAware += deltatime;
		TimeHostile = 0;
	}
	else {
		DrawDebugLineBetweenActors(PlayerLocation, colorHOSTILE);
		UE_LOG(LogTemp, Display, TEXT("TimeHostile %f"), TimeHostile);
		TimeHostile += deltatime;
		TimeAware = 0;
	}
	if (TimeHostile > HostileTimer/2) {
		UE_LOG(LogTemp, Display, TEXT("%s is HOSTILE!"), *GetName());
		States = HOSTILE;
		TimeAware = 0;
	}
	if (TimeAware > AwareTimer) {
		UE_LOG(LogTemp, Display, TEXT("%s is IDLE again"), *GetName());
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




