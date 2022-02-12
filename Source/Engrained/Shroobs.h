// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include <Shape.h>
#include "GameFramework/Actor.h"
#include "Shroobs.generated.h"

UCLASS()
class ENGRAINED_API AShroobs : public AActor
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

	/* Lagrer origo til movArea */
	FVector MoveAreaVector;
	FVector spawnPoint;
	FVector CurrentLocation;
	
	FRotator newRotation;
	FRotator spawnRotation;
	FRotator oldRotation;
	FRotator RotationBack;

	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 70;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationPointRandomRange = 60.f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Rotation_Z;
	float Private_Rotation_Z;
	int InitializeRotation{ 0 };	// Rotation only starts when at 0

	float dotProduct_Right;


	float Seconds;
	float timeSinceRotation = 0.f;
	float timeToGoBack = 10.f;
	float timeToChangeDirection = 3.f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool RayTraceActive{ false };
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RayTraceLength{ 0.f };
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RayTraceTiming{ 0.5f };
	float RayTraceSeconds;

	bool bRotateBack{ false };
	

	int amountOfRotation = 0;

	bool test = false;
	
	void RandRotation();

	//void SpawnVolume();

	UFUNCTION()
	void onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	UPROPERTY(Editanywhere, Category = "Navigation")
	class AEnemySpawner* movArea{ nullptr };
	AActor* movAreaActor{ nullptr };

	/* Raytracer in z axis, collides only with 'EnemyAreaObj' Object type. Found under collision presets */
	FHitResult RayTracer(float range);
	/* Checks if RayTracer hit the specified enemy movement area */
	void MoveAreaCheck(float range);
	/* Rotates actor towards FVector */
	void RotateToVector();
	/* Returns dotproduct of 2 vectors */
	float dotProduct(FVector vec1, FVector vec2);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//TSubclassOf<BoxShape> ActorToSpawn;


};