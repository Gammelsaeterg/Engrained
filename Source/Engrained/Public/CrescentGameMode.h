// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrescentGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ENGRAINED_API ACrescentGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	void HandleGameStart();

	void HandleGameOver(bool PlayerWon);

	TSubclassOf<APawn> ClassToFind;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<APawn*> ScenePawns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> ScenePawnsLocations;



public:

	void ActorDied(AActor* DeadActor);

	UFUNCTION(BlueprintCallable)
	void RememberPawn(APawn* pawn);

	UFUNCTION(BlueprintCallable)
	void RespawnPawns();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
		void GameOver(bool PlayerWond);

	UFUNCTION(BlueprintCallable)
		void GetScenePawns();

	UFUNCTION(BlueprintImplementableEvent)
		void RespawnPawnsEvent();

};
