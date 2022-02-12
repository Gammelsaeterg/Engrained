// The game and source code belongs to Team 7 Programming team


#include "Sticky_Platform.h"

// Sets default values
ASticky_Platform::ASticky_Platform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASticky_Platform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASticky_Platform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

