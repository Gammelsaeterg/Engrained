// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EngrainedGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ENGRAINED_API UEngrainedGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MasterVolume{ 80.f };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float EffectsVolume{ 80.f };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MusicVolume{ 80.f };
};
