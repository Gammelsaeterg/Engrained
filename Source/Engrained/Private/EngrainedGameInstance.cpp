// The game and source code belongs to Team 7 Programming team


#include "EngrainedGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UEngrainedGameInstance::SetSpawnPoint(AActor* Spawn)
{
	SpawnPoint = Spawn;
	UE_LOG(LogTemp, Warning, TEXT("%s as spawnpoint"), *SpawnPoint->GetName());
}

AActor* UEngrainedGameInstance::ReturnSpawnPoint()
{
	return SpawnPoint;
}

//void UEngrainedGameInstance::PlayerRespawn()
//{
//}
