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

	//spawnPoint = GetActorLocation();
	//spawnRotation = GetActorRotation();
	
//	ShroobSensingSphere->onOverlap.AddDynamic(this, &AShroobs::onOverlap);
}

// Called every frame
void AShroobs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Seconds += DeltaTime;

	CurrentLocation = GetActorLocation();
	CurrentLocation += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(CurrentLocation);
	
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
}

void AShroobs::ImHit()
{
	Destroy();
}



void AShroobs::onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	//AMina* Player = Cast<AMina>(OtherActor);

	//if(Player)
	//{
	//	FVector MinaLocation = Player->MinaCurrentLocation;
	//	RotationBack = FRotationMatrix::MakeFromX(MinaLocation-GetActorLocation()).Rotator();
	//	SetActorRotation(RotationBack);
	//	test = true;
	//}
}
