// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "NonPlayerActorBase.h"
#include "Whaleshroom.generated.h"

/**
 * 
 */
UCLASS()
class ENGRAINED_API AWhaleshroom : public ANonPlayerActorBase
{
	GENERATED_BODY()
	
public:
	AWhaleshroom();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

};
