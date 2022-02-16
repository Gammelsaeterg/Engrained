// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

// class UCapsuleComponent;		// Vil helst ha en cylinder eller circle shape for collision
//class USceneComponent;
class UBoxComponent;
class UTextRenderComponent;
class AShroobs;
class AActor;

UCLASS()
class ENGRAINED_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//USceneComponent* Root;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* Spawner;

	//AShroobs* Shroob{ nullptr };

	//UPROPERTY(EditDefaultsOnly, Catergory = "Spawning")
	//TSubclassOf<AActor> ActorToSpawn;
};
