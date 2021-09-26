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
	spawnPoint = GetActorLocation();
	spawnRotation = GetActorRotation();
	

}


// Called every frame
void AShroobs::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Seconds += DeltaTime;
	CurrentLocation = GetActorLocation();

	CurrentLocation += GetActorForwardVector() * forwardSpeed * DeltaTime;
	SetActorLocation(CurrentLocation);

	
if(test == false)
{
	if(amountOfRotation <3)
	{
		if(Seconds > timeToChangeDirection)
		{
			AShroobs::RandRotation();
			SetActorRotation(newRotation);
		}
	}else if(CurrentLocation == spawnPoint)
	{
		
		//amountOfRotation = 0;
	}else if(CurrentLocation != spawnPoint)
	{
		RotationBack = FRotationMatrix::MakeFromX(spawnPoint-GetActorLocation()).Rotator();
		SetActorRotation(RotationBack);
		
	}
}
}

void AShroobs::ImHit()
{
	Destroy();
}

void AShroobs::RandRotation()
{
		Seconds = 0.f;
	
		rotator_Z =  FMath::RandRange(-90.f, 90.f);
		newRotation = FRotator(0,rotator_Z,0);
		UE_LOG(LogTemp, Warning, TEXT("test,test"))
		amountOfRotation++;
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
