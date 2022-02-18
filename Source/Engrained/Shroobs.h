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


	



	UFUNCTION()
	void onOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:    
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};