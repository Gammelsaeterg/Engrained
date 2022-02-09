// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sticky_Platform.generated.h"

UCLASS()
class ENGRAINED_API ASticky_Platform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASticky_Platform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
