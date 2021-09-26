// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrescentGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CRYSTALGAMECORRECT_API ACrescentGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	void HandleGameStart();

	void HandleGameOver(bool PlayerWon);

public:

	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool PlayerWond);

};
