// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CrystalCluster.generated.h"


UCLASS()
class CRYSTALGAMECORRECT_API ACrystalCluster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystalCluster();

	void IsStruck();

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	TSubclassOf<AActor> ActorToSpawn;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Cluster")
		class UBoxComponent* CrystalCollider;

	UPROPERTY(EditAnywhere, Category = "Cluster")
		USkeletalMeshComponent* CrystalVisibleMesh;

	UFUNCTION()
	void SpawnObject();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
