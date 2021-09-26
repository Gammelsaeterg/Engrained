// The game and source code belongs to Team 7 Programming team


#include "CrystalCluster.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup.h"

// Sets default values
ACrystalCluster::ACrystalCluster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	

	//collider
	CrystalCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("CrystalCollider"));
	RootComponent = CrystalCollider;

    //Set up our visible mesh
	CrystalVisibleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CrystalMesh"));
	CrystalVisibleMesh->SetupAttachment(RootComponent);
}



// Called when the game starts or when spawned
void ACrystalCluster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrystalCluster::IsStruck()
{
	SpawnObject();
	Destroy();
}

void ACrystalCluster::SpawnObject()
{
	FActorSpawnParameters SpawnParams;
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();
	AActor* SpawnedActorRef = GetWorld()->SpawnActor<APickup>(ActorToSpawn, Loc, Rot, SpawnParams);
}

// Called every frame
void ACrystalCluster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

