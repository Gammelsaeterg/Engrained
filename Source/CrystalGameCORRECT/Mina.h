// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Mina.generated.h"

class ACrystalProjectile;

UCLASS()
class CRYSTALGAMECORRECT_API AMina : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMina();

	//Components for Mina
	UPROPERTY(EditAnywhere, Category = "SpringArm")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere, Category = "CameraComp")
	class UCameraComponent* CameraComp;

	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ACrystalProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Projectile");
	USceneComponent* ProjectileSpawnPoint;

	//Dash Particles
	UPROPERTY(EditAnywhere, Category = "Particle Effects")
		UNiagaraSystem* DashParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* AttackBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		FVector AttackPlacement {70.f, 0.f, 35.f};

	//Mina turning speed
    UPROPERTY(EditAnywhere, Category = "MinaTurning")
		float BaseTurnRate;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnAttackBeginOverlap();

	UFUNCTION(BlueprintCallable)
		void OnAttackEndOverlap();

	UFUNCTION()
		void Refill();

	FVector MinaCurrentLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	FORCEINLINE class UCameraComponent* GetCameraComp() const { return CameraComp; }


	//Movng forward
	UFUNCTION()
		void Forward(float Value);
	//Moving right
	UFUNCTION()
		void Right(float Value);

	//Called via input to turn at a a given rate
	UFUNCTION()
		void TurnAtRate(float Rate);

	//Jumpong Functions
	UFUNCTION()
		void CheckJump();
	UPROPERTY()
		bool Jumping;

	//Mellee attacks and projectile attacks below
	UFUNCTION(BlueprintCallable)
		void Melee();
	UFUNCTION(BlueprintCallable)
		void StopMelee();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool isAttacking = false;
	//Attacks done

	//Shooting code
	UPROPERTY()
		float Damage;
	UFUNCTION()
		void Shoot();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
		int32 CrystalAmmo { 0 };
	//Shooting code done

	//Dash Functions and porperty
	//Length of desh, speed of dash, dash stop timer etc.
	UFUNCTION()
		void Dash();
	UPROPERTY(EditAnywhere)
		float DashDistance;
	UPROPERTY(EditAnywhere)
		float DashCooldown;
	UPROPERTY()
		bool CanDash;
	UPROPERTY(EditAnywhere)
		float DashStop;
	UPROPERTY()
		FTimerHandle UnusedHandle;
	UFUNCTION()
		void StopDashing();
	UFUNCTION()
		void ResetDash();
	//End of dash functions
};
