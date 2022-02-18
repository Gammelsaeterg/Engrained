// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NonPlayerActorBase.generated.h"

UCLASS()
class ENGRAINED_API ANonPlayerActorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANonPlayerActorBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	/* Lagrer origo til movArea */
	FVector MoveAreaVector;
	FVector CurrentLocation;

	/* Movement Variables */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 70;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationPointRandomRange = 60.f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Rotation_Z;
	float Private_Rotation_Z;
	int InitializeRotation{ 0 };	// Character only when this is at 0
	float dotProduct_Right;//Helper variable for rotation

	/* Raytracer variables */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool RayTraceActive{ false };
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RayTraceLength{ 0.f };
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RayTraceTiming{ 0.5f };
	float RayTraceSeconds;


	/* Activate debugline to match raytracer from actor */
	UPROPERTY(EditAnywhere, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	bool bDebugLine{ false };

	bool bRotateBack{ false };
	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Actor's components */
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* ShroobSensingSphere{ nullptr };
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ShroobVisibleMesh;

	UPROPERTY(Editanywhere, Category = "Navigation")
	class AEnemySpawner* movArea{ nullptr };
	AActor* movAreaActor{ nullptr };

	/* Raytracer in z axis, collides only with 'EnemyAreaObj' Object type */
	FHitResult RayTracer(float range);
	/* Checks if RayTracer hit the specified enemy movement area */
	void MoveAreaCheck(float range);
	/* Rotates actor towards FVector */
	void RotateToVector();
	/* Returns dotProduct of 2 vectors in X , Y */
	float dotProduct2D(FVector vec1, FVector vec2);




	virtual void HandleDestruction();
};
