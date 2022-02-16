// Fill out your copyright notice in the Description page of Project Settings.


#include "Shroobs.h"

#include <chrono>

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Health.h"
#include "Mina.h"
#include "NavigationSystemTypes.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"
#include "EnemySpawner.h"
#include "DrawDebugHelpers.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "PxMathUtils.h"


// Sets default values
AShroobs::AShroobs()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the collider for the shroob
	ShroobCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShroobCollider"));
	ShroobCollider->InitBoxExtent(FVector(50.f));

	//Sensing Sphere. checking if there is a player near by
	ShroobSensingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShroobSenesingSphere"));
	ShroobSensingSphere->SetupAttachment(GetRootComponent());
	ShroobSensingSphere->InitSphereRadius(700.f);

	// Set OurCollider to be the RootComponent
	RootComponent = ShroobCollider;

	// Set up our visible mesh
	ShroobVisibleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShroobVisibleMesh"));
	ShroobVisibleMesh->SetupAttachment(RootComponent);
    

}

// Called when the game starts or when spawned
void AShroobs::BeginPlay()
{
	Super::BeginPlay();

	//spawnPoint = GetActorLocation();
	//spawnRotation = GetActorRotation();
	
//	ShroobSensingSphere->onOverlap.AddDynamic(this, &AShroobs::onOverlap);

	if (movArea) {
		UE_LOG(LogTemp, Warning, TEXT("%s Found: %s"), *GetName(), *movArea->GetName());
		movAreaActor = Cast<AActor>(movArea);
		MoveAreaVector = movAreaActor->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("MoveAreaVector: %s"), *MoveAreaVector.ToString());
		UE_LOG(LogTemp, Warning, TEXT("%s ForwardVector: %s"), *GetName(),*GetActorForwardVector().ToString());
	}

}

FHitResult AShroobs::RayTracer(float range)
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, this);

	GetWorld()->LineTraceSingleByObjectType(
		Hit,
		this->GetActorLocation(),
		this->GetActorLocation() - FVector(0, 0, range),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
		TraceParams
	);
	return Hit;
}

void AShroobs::MoveAreaCheck(float range)
{
	FHitResult HitResult = RayTracer(range);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName());
		if (ActorHit == movAreaActor) {
			/* Actor is over its movement area */
			Private_Rotation_Z = 0;
			bRotateBack = false;
			InitializeRotation = 0;
			UE_LOG(LogTemp, Warning, TEXT("%s is currently over %s"), *GetName(), *movArea->GetName());
		}
		else {
		}
	}
	else {
		/* Actor is NOT over its movement area */
		UE_LOG(LogTemp, Error, TEXT("%s is currently over nothing"), *this->GetName());
		//RotateToVector();
		Private_Rotation_Z = Rotation_Z;
		bRotateBack = true;
	}
}

/* Currently only rotates towards origo of MoveAreaVector */
void AShroobs::RotateToVector()
{
	/* Randomizes MoveAreaVector to give some variation */
	FVector MoveToVector = MoveAreaVector + FVector(FMath::RandRange(
		-RotationPointRandomRange, RotationPointRandomRange), 
		FMath::RandRange(-RotationPointRandomRange, RotationPointRandomRange), 0);
	//UE_LOG(LogTemp, Warning, TEXT("MoveAreaVector: %s"), *MoveAreaVector.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MoveTo: %s"), *MoveToVector.ToString());

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Current = GetActorLocation();
	FVector Turn = MoveToVector - Current;
	//UE_LOG(LogTemp, Warning, TEXT("Forward: %s"), *Forward.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Turn: %s"), *Turn.ToString());

	float dotProduct_Forward = dotProduct(Forward, Turn);
	//UE_LOG(LogTemp, Error, TEXT("Dotproduct_forward: %f"), dotProduct_Forward);
	if (!InitializeRotation) {
		dotProduct_Right = dotProduct(Right, Turn);
		InitializeRotation++;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Dotproduct_right  : %f"), dotProduct_Right);
	
	//UE_LOG(LogTemp, Error, TEXT("Angle %f"), angle);

	float rotationStrength = (
		(acosf(dotProduct_Forward) * (180/PI)) / 
		180
		);
	//UE_LOG(LogTemp, Error, TEXT("RotationStrength: %f"), rotationStrength);

	if (dotProduct_Forward < 1) {
		if (dotProduct_Right >= 0)
			AddActorLocalRotation(FRotator(0, Private_Rotation_Z * rotationStrength, 0));
		else if (dotProduct_Right < 0)
			AddActorLocalRotation(FRotator(0, -Private_Rotation_Z * rotationStrength, 0));
	}
}

float AShroobs::dotProduct(FVector vec1, FVector vec2)
{
	float a = vec1.X * vec2.X + vec1.Y * vec2.Y;
	float b = FGenericPlatformMath::Sqrt(
		FGenericPlatformMath::Pow(vec1.X, 2) +
		FGenericPlatformMath::Pow(vec1.Y, 2)
	);
	float c = FGenericPlatformMath::Sqrt(
		FGenericPlatformMath::Pow(vec2.X, 2) +
		FGenericPlatformMath::Pow(vec2.Y, 2)
	);

	return (a / (b * c));
}


// Called every frame
void AShroobs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Seconds += DeltaTime;

	CurrentLocation = GetActorLocation();
	CurrentLocation += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(CurrentLocation);
	
	RayTraceSeconds += DeltaTime;
	if (RayTraceSeconds > RayTraceTiming) {
		if (RayTraceActive) {
			MoveAreaCheck(RayTraceLength);
			
			/* Draws debugline matching the raytrace */
			DrawDebugLine(
				GetWorld(),
				this->GetActorLocation(),
				this->GetActorLocation() - FVector(0, 0, RayTraceLength),
				FColor(255, 0, 0),
				false,
				0.f,
				0,
				1.f
			);
		}
		RayTraceSeconds = 0;
	}

	if (bRotateBack) {


		RotateToVector();
	}
	
	//if(test == false)
	//{
	//	if(amountOfRotation <3)
	//	{
	//		if(Seconds > timeToChangeDirection)
	//		{
	//			AShroobs::RandRotation();
	//			SetActorRotation(newRotation);
	//		}
	//	}else if(CurrentLocation == spawnPoint)
	//	{
	//	
	//		//amountOfRotation = 0;
	//	}else if(CurrentLocation != spawnPoint)
	//	{
	//		RotationBack = FRotationMatrix::MakeFromX(spawnPoint-GetActorLocation()).Rotator();
	//		SetActorRotation(RotationBack);
	//	
	//	}
	//}
}

void AShroobs::ImHit()
{
	Destroy();
}

void AShroobs::RandRotation()
{
		//Seconds = 0.f;
	
		//rotator_Z =  FMath::RandRange(-90.f, 90.f);
		//newRotation = FRotator(0,rotator_Z,0);
		////UE_LOG(LogTemp, Warning, TEXT("test,test"))
		//amountOfRotation++;
}




void AShroobs::onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	AMina* Player = Cast<AMina>(OtherActor);

	if(Player)
	{
		FVector MinaLocation = Player->MinaCurrentLocation;
		RotationBack = FRotationMatrix::MakeFromX(MinaLocation-GetActorLocation()).Rotator();
		SetActorRotation(RotationBack);
		test = true;
	}
}
