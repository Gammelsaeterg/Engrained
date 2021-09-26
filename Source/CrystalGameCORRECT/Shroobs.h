// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include <Shape.h>
#include "GameFramework/Actor.h"
#include "Shroobs.generated.h"

UCLASS()
class CRYSTALGAMECORRECT_API AShroobs : public AActor
{
	GENERATED_BODY()
    
	public:    
	// Sets default values for this actor's properties
	AShroobs();
    
	void ImHit();


	protected:

    

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* ShroobCollider;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* ShroobSensingSphere{nullptr};

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ShroobVisibleMesh;

	FVector spawnPoint;
	FVector CurrentLocation;
	
	FRotator newRotation;
	FRotator spawnRotation;
	FRotator oldRotation;
	FRotator RotationBack;

	float rotator_Z;
	float forwardSpeed = 70;

	float Seconds;
	float timeSinceRotation = 0.f;
	float timeToGoBack = 10.f;
	float timeToChangeDirection = 3.f;

	int amountOfRotation = 0;

	bool test = false
	;
	
	void RandRotation();

	//void SpawnVolume();

	UFUNCTION()
	void onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TSubclassOf<BoxShape> ActorToSpawn;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};