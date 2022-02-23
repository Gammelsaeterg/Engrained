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
	switch (states) {
	case IDLE:	// Idle
		break;
	case SHOCK:	// Shocked
		/* Skal gjøre en sjekk når den entrer SHOCK for å se om spilleren er der. 
		*	Etter at shock timeren har gått ut så gjør den en til sjekk, 
		*	hvis spilleren da fortsatt er der, så går den over til HOSTILE
		*	Hvis ikke så går den til enten IDLE eller AWAREOFPLAYER */
		UE_LOG(LogTemp, Display, TEXT("Shocked: %f"), TimeShocked);
		TimeShocked += deltatime;
		if (TimeShocked > ShockTimer) {
			UE_LOG(LogTemp, Display, TEXT("%s is Hostile!"), *GetName());
			states = HOSTILE;
		}
		break;
	case HOSTILE:	// Hostile
		break;
	case AWAREOFPLAYER:	// Aware of player
		break;
	default:
		break;
	}
}

// Called every frame
void AShroobs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Seconds += DeltaTime;
	
	CurrentLocation = GetActorLocation();
	CurrentLocation += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(CurrentLocation);
	
	ActorState(DeltaTime);

	//UE_LOG(LogTemp, Display, TEXT("Rotate: %s"), (bRotateBack ? TEXT("true") : TEXT("false")));
	/* Movement across platform */
	RayTraceSeconds += DeltaTime;
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
		DetectPlayer(DeltaTime);

	if (bDetectPlayer) {
		if (TimeDetected > DetectionTimer) {
		}
	}
	else {
		//TimeDetected = 0;
	}
}

void AShroobs::ImHit()
{
	Destroy();
}




