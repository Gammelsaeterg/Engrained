// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NonPlayerActorBase.h"
//#include <Shape.h>
#include "GameFramework/Actor.h"
#include "Shroobs.generated.h"

UCLASS()
class ENGRAINED_API AShroobs : public ANonPlayerActorBase
{
	GENERATED_BODY()
    
public:    
	// Sets default values for this actor's properties
	AShroobs();
    
	void ImHit();


protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ActorState(float deltatime) override;


public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};