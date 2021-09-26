// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrystalProjectile.generated.h"


UCLASS()
class CRYSTALGAMECORRECT_API ACrystalProjectile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACrystalProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//speed of the bullet
	UPROPERTY(EditAnywhere, Category = "Setup")
		float Speed{ 2000.f };

	//We destroy the bullets after 5 seconds
	UPROPERTY(EditAnywhere, Category = "Setup")
		float TimeBeforeDestroy{ 3.f };

	float TimeLived{ 0 };


	//the Bullets collider which can be edited in the Editor
	UPROPERTY(EditAnywhere, Category = "Setup");
	class USphereComponent* OurCollider{ nullptr };

	//the Visual mesh of the actor
	UPROPERTY(EditAnywhere, Category = "Setup");
	class UStaticMeshComponent* OurVisibleComponent{ nullptr };


	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

public:
	
	virtual void Tick(float DeltaTime) override;

};
