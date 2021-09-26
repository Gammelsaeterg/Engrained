// Fill out your copyright notice in the Description page of Project Settings.


#include "CrescentGameMode.h"

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


