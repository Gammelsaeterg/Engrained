// The game and source code belongs to Team 7 Programming team


#include "NonPlayerActorBase.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "EnemySpawner.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "PxMathUtils.h"
#include "DrawDebugHelpers.h"

// Sets default values
ANonPlayerActorBase::ANonPlayerActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the collider for the shroob
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	BoxCollider->InitBoxExtent(FVector(50.f));

	// Set OurCollider to be the RootComponent
	RootComponent = BoxCollider;

	//Sensing Sphere. checking if there is a player near by
	ShroobSensingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShroobSenesingSphere"));
	ShroobSensingSphere->SetupAttachment(GetRootComponent());
	ShroobSensingSphere->InitSphereRadius(700.f);

	// Set up our visible mesh
	ShroobVisibleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShroobVisibleMesh"));
	ShroobVisibleMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ANonPlayerActorBase::BeginPlay()
{
	Super::BeginPlay();
	
	/* Finds area actor is supposed to move across */
	if (movArea) {
		movAreaActor = Cast<AActor>(movArea);
		MoveAreaVector = movAreaActor->GetActorLocation();
	}
}

// Called every frame
void ANonPlayerActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Forward = GetActorForwardVector();

	//Forward = Forward / Forward;
	//UE_LOG(LogTemp, Warning, TEXT("Forward: %s"), *Forward.ToString());

	DrawDebugLine(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + (FVector(100, 0, 0) * Forward),
		FColor(0, 255, 0),
		false,
		0,
		0,
		1.f
	);
}


FHitResult ANonPlayerActorBase::RayTracer(float range)
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

void ANonPlayerActorBase::MoveAreaCheck(float range)
{
	FHitResult HitResult = RayTracer(range);

	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		if (ActorHit == movAreaActor) {
			/* Actor is over its movement area */
			Private_Rotation_Z = 0;
			bRotateBack = false;
			InitializeRotation = 0;
		}
		else {
		}
	}
	else {
		/* Actor is NOT over its movement area */
		Private_Rotation_Z = Rotation_Z;
		bRotateBack = true;
	}
}

/* Currently only rotates towards origo of MoveAreaVector */
void ANonPlayerActorBase::RotateToVector()
{
	/* Randomizes MoveAreaVector to give some variation */
	FVector MoveToVector = MoveAreaVector + FVector(FMath::RandRange(
		-RotationPointRandomRange, RotationPointRandomRange),
		FMath::RandRange(-RotationPointRandomRange, RotationPointRandomRange), 0);

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector Current = GetActorLocation();
	FVector Turn = MoveToVector - Current;

	float dotProduct_Forward = dotProduct2D(Forward, Turn);
	if (!InitializeRotation) {
		dotProduct_Right = dotProduct2D(Right, Turn);
		InitializeRotation++;
	}

	float rotationStrength = (
		(acosf(dotProduct_Forward) * (180 / PI)) /
		180
		);

	if (dotProduct_Forward < 1) {
		if (dotProduct_Right >= 0)
			AddActorLocalRotation(FRotator(0, Private_Rotation_Z * rotationStrength, 0));
		else if (dotProduct_Right < 0)
			AddActorLocalRotation(FRotator(0, -Private_Rotation_Z * rotationStrength, 0));
	}
}

float ANonPlayerActorBase::dotProduct2D(FVector vec1, FVector vec2)
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

// What happens on this actor being destroyed
void ANonPlayerActorBase::HandleDestruction()
{
}

