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

	SpawnPoints.Empty();
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
	PlayerCameraRotation.Yaw += Val * 70 * GetWorld()->GetDeltaSeconds(); //TODO: Customizable variable, yaw scale
}

void AMinaExperimental::AddPitchInput(float Val)
{
	//if ((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) > 90.f)
	//{
	//	PlayerCameraRotation.Pitch = 90.f;
	//}
	//else if ((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) < -90.f)
	//{
	//	PlayerCameraRotation.Pitch = -90.f;
	//}
	//else if ((!((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) > 90.f) &&
	//	      !((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) < -90.f)))
	//{
	//	PlayerCameraRotation.Pitch -= Val * 45.f * GetWorld()->GetDeltaSeconds(); //TODO: Customizable variable, pitch scale
	//}

	if ((!((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) > 90.f) &&
		!((PlayerCameraRotation.Pitch - Val * 45.f * GetWorld()->GetDeltaSeconds()) < -90.f)))
	{
		PlayerCameraRotation.Pitch -= Val * 45.f * GetWorld()->GetDeltaSeconds(); //TODO: Customizable variable, pitch scale
	}
}

void AMinaExperimental::InsertSpawnPoint(AActor* Spawn)
{
	for (auto it : SpawnPoints) {
		if (it == Spawn) {
			//UE_LOG(LogTemp, Warning, TEXT("SpawnPoint: %s : has already been pushed"), *Spawn->GetName());
			return;
		}
	}
	SpawnPoints.Push(Spawn);
	//UE_LOG(LogTemp, Warning, TEXT("Pushed Spawn: %s"), *Spawn->GetName());
}

AActor* AMinaExperimental::FindClosestSpawn()
{
	AActor* ReturnActor = nullptr;
	float Length{ 1000000 }, prev{}, tmp{};
	FVector MinaLocation = GetActorLocation();

	if (SpawnPoints.Num() == 0) {
		//UE_LOG(LogTemp, Display, TEXT("No spawn points available"));

		return ReturnActor;
	}

	for (auto it : SpawnPoints) {
		//UE_LOG(LogTemp, Display, TEXT("iterator:: %s"), *it->GetName());

		FVector SpawnLocation = it->GetActorLocation();
		tmp = LengthBetweenVectors(MinaLocation, SpawnLocation);
		//UE_LOG(LogTemp, Display, TEXT("Length between %s and Mina is %f"), *it->GetName(), tmp);

		if (tmp < Length) {
			Length = tmp;
			ReturnActor = it;
		}
	}

	//if (ReturnActor) {
	//	UE_LOG(LogTemp, Warning, TEXT("Found: %s : as respawnpoint"), *ReturnActor->GetName());
	//}
	//else
	//	UE_LOG(LogTemp, Error, TEXT("No respawn point found"));

	return ReturnActor;
}

void AMinaExperimental::RespawnPlayer()
{
	AActor* Respawnpoint = FindClosestSpawn();
	if (Respawnpoint)
		SetActorLocation(Respawnpoint->GetActorLocation() + FVector(0, 0, 200));
}

void AMinaExperimental::SetInitialSpawn()
{
	//Respawnpoint = this->GetActorLocation();
}

float AMinaExperimental::LengthBetweenVectors(FVector vec1, FVector vec2)
{
	FVector v = vec1 - vec2;
	return sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
}