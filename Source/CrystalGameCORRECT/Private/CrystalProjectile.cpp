// The game and source code belongs to Team 7 Programming team


#include "CrystalProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.H"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CrystalGameCORRECT/Shroobs.h"
#include "CrystalGameCORRECT/Mina.h"
#include "CrystalCluster.h"

// Sets default values
ACrystalProjectile::ACrystalProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OurCollider = CreateDefaultSubobject<USphereComponent>(TEXT("MyCollider"));
	RootComponent = OurCollider;

	//Getting the visual mesh attached
	OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
	OurVisibleComponent->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ACrystalProjectile::BeginPlay()
{
	Super::BeginPlay();

	Cast<USphereComponent>(RootComponent)->OnComponentBeginOverlap.AddDynamic(this, &ACrystalProjectile::OnOverlap);

}


void ACrystalProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeLived += DeltaTime;
	if (TimeLived > TimeBeforeDestroy)
	{
		this->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Destroyed"))
	}

	FVector NewLocation = GetActorLocation();
	NewLocation += GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(NewLocation);
}


void ACrystalProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->IsA(AMina::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Crystal overlap %s"), *OtherActor->GetName())
		return;
	}
	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Crystal overlap %s"), *OtherActor->GetName())
		Destroy();
	}
	if (OtherActor->IsA(AShroobs::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Crystal overlap %s"), *OtherActor->GetName())
		Cast<AShroobs>(OtherActor)->ImHit();
		//Destroy crystal
		Destroy();
	}
	if (OtherActor->IsA(ACrystalCluster::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Crystal Overlap %s"), *OtherActor->GetName())
		Cast<ACrystalCluster>(OtherActor)->IsStruck();
		Destroy();
	}
}
