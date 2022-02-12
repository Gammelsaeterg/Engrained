// The game and source code belongs to Team 7 Programming team


#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/SceneComponent.h"
#include "../Shroobs.h"
#include "GameFramework/Actor.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	//RootComponent = Root;

	Spawner = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawner"));
	//Spawner->SetupAttachment(Root);
	RootComponent = Spawner;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	//AActor* UWorld::SpawnActor(AShroobs);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

