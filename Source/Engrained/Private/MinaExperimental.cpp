// The game and source code belongs to Team 7 Programming team


#include "MinaExperimental.h"

// Sets default values
AMinaExperimental::AMinaExperimental()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

// Called when the game starts or when spawned
void AMinaExperimental::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMinaExperimental::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerCameraRotation.Normalize();
}

// Called to bind functionality to input
void AMinaExperimental::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMinaExperimental::AddYawInput(float Val)
{
	PlayerCameraRotation.Yaw += Val * 45 * GetWorld()->GetDeltaSeconds(); //TODO: Customizable variable, yaw scale
}

void AMinaExperimental::AddPitchInput(float Val)
{
	if ((!((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) > 90.f) &&
		 !((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) < -90.f)))
	{
		PlayerCameraRotation.Pitch -= Val * 45.f * GetWorld()->GetDeltaSeconds(); //TODO: Customizable variable, pitch scale
	}

}

