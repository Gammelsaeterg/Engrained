// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MinaExperimental.generated.h"

UCLASS()
class ENGRAINED_API AMinaExperimental : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMinaExperimental();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	FRotator PlayerCameraRotation{0.f, 0.f, 0.f};

	UFUNCTION(BlueprintCallable)
	void AddYawInput(float Val);

	UFUNCTION(BlueprintCallable)
	void AddPitchInput(float Val);
};
