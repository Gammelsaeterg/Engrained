// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "CrystalProjectile.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ENGRAINED_API APickup : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	ACrystalProjectile* CrystalProjectile;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* PickupRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditAnywhere)
	UShapeComponent* PickupBox;

	UPROPERTY(EditAnywhere, Category = "PickupType")
	bool Ammo;

	UPROPERTY(EditAnywhere, Category = "PickupType")
	bool Health;

	UPROPERTY(EditAnywhere)
	float HealthToAdd = 0;

	//Function we call when pickup is overlapped by actor
	UFUNCTION()
	void OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
