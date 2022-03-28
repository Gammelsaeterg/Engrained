// Fill out your copyright notice in the Description page of Project Settings.


#include "CrescentGameMode.h"
#include "Kismet/GameplayStatics.h"

void ACrescentGameMode::BeginPlay()
{

}

void ACrescentGameMode::ActorDied(AActor* DeadActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Actor has been killed"))
}

void ACrescentGameMode::HandleGameStart()
{

}

void ACrescentGameMode::HandleGameOver(bool PlayerWon)
{

}

void ACrescentGameMode::RememberPawn(APawn* pawn)
{
	ScenePawns.Add(pawn);
	ScenePawnsLocations.Add(pawn->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("Gamemode remembering: %s"), *pawn->GetName());
}

void ACrescentGameMode::RespawnPawns()
{
	RespawnPawnsEvent();

}

void ACrescentGameMode::GetScenePawns()
{
	//for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	// Follow iterator object to my actual actor pointer
	//	AActor* MyActor = *ActorItr;
	//}

	const UWorld* MyWorld = GetWorld();

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, ScenePawns);
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, APawn::StaticClass());
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), ScenePawns);

	//TArray<APawn*> pawns;
	//UGameplayStatics::GetAllActorsOfClass

}




