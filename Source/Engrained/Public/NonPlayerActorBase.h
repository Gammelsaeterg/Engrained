// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "NonPlayerActorBase.generated.h"

#define IDLE 0
#define SHOCK 1
#define HOSTILE 2
#define AWAREOFPLAYER 3
#define DEATH 10

#define colorIDLE FColor{0, 255, 0}
#define colorSHOCK FColor{255, 255, 0}
#define colorHOSTILE FColor{255, 0, 0}
#define colorAWAREOFPLAYER FColor{0, 0, 255}

UCLASS()
class ENGRAINED_API ANonPlayerActorBase : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANonPlayerActorBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	

	/* States for actor 
	* 0 = idle
	* 1 = Shock
	* 2 = Hostile
	* 3 = Aware of player */
	int States{ 0 };
	FColor StateColor;
	void ShowStateColor();	// DEBUG

	FVector MoveAreaVector;
	FVector CurrentLocation;
	//FVector Length;
	float Length{ 1.f };

	/* Using FVector as movement target */
	FVector MoveToVector;


	FVector PlayerLocation{};

	/* Testing field of vision */
	FVector Line;

	/* Movement Variables */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed = 70;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RotationPointRandomRange = 60.f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Rotation_Z;
	float Private_Rotation_Z;
	int FindRandomArea{ 0 }; // If at one, actor will find random area when instructed
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

	/* Player detection */
	/* For debuging. Whether the actor has player detection enabled */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	bool bDebugPlayerDetection{ false };
	bool bDetectPlayer{ false };
	/* Total angle of detection */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float DetectionAngle{ 100.f };
	/* Length of actor player detection */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float DetectionLength{ 500.f };
	/* Time it takes for actor to detect player */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float DetectionTimer{ 0.5f };
	float TimeDetected{ 0 };

	//bool bHostile{ false };
	//bool bAwareofPlayer{ false };

	/* When actor detects player while IDLE it will be shocked for x-time */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float ShockTimer{ 1.f };
	float TimeShocked{ 0 };

	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float HostileReach{ 600.f };
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float HostileTimer{ 2.f };
	float TimeHostile{ 0 };
	
	/* How long the actor will stay in the AWAREOFPLAYER state before going back to IDLE */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float AwareTimer{ 3.f };
	float TimeAware{ 0.f };
	/* How often the actor will move when in the AWAREOFPLAYER state */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float AwareMovementTimer{ 2.f };
	float TimeAwareMovement = AwareMovementTimer / 2;
	//UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	//float AwareRandomMovementLength{ 100.f };
	/* Actors Right Vector * AwareMovementSpeed */
	UPROPERTY(EditAnywhere, Category = "Player detection and hostility", meta = (AllowPrivateAccess = "true"))
	float AwareMovementSpeed{ 2.f };


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Actor's components */
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxCollider;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SensingSphere{ nullptr };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* VisibleMesh;

	UPROPERTY(Editanywhere)
	class AEnemySpawner* movArea{ nullptr };
	AActor* movAreaActor{ nullptr };	


	void DrawDebugLineBetweenActors(FVector OtherActor, FColor color);
	/* Raytracer in z axis, collides only with 'EnemyAreaObj' Object type */
	FHitResult RayTracer(float range);
	/* Checks if RayTracer hit the specified enemy movement area */
	void MoveAreaCheck(float range);
	/* Rotates actor towards FVector */
	void RotateToVector();
	void RotateToVector(FVector TowardsVector, float Rotation);
	/* Returns dotProduct of 2 vectors in X , Y */
	float dotProduct2D(FVector vec1, FVector vec2);


	void DetectPlayer(float deltatime);
	bool OtherActorWithinReach(FVector OtherActor, float reach);

	/* State machine for actor */
	virtual void ActorState(float deltatime);
	/* Base functions for the various States of the actor */
	virtual void ActorIDLE(float deltatime);
	virtual void ActorSHOCK(float deltatime);
	virtual void ActorHOSTILE(float deltatime);
	virtual void ActorAWAREOFPLAYER(float deltatime);

	virtual void HandleDestruction();

	UFUNCTION()
	void onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void endOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherBodyIndex);

private:
	//UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//class AEnemySpawner* movArea{ nullptr };
	//AActor* movAreaActor{ nullptr };

	//UPROPERTY(EditAnywhere)
	//class AMina* mina;
	AActor* player;

	float VectorMagnitude(FVector vec);
};
