// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"
#include "CrescentGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CrystalGameCORRECT/Mina.h"
#include "Pickup.h"



// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	

}




// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();

	Health = DefaultHealth;
	GameModeRef = Cast<ACrescentGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealth::TakeDamage);
}



void UHealth::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Mina has taken damage"))
	if (Damage == 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	if (Health <= 0)
	{
			if (DamagedActor->IsA(AMina::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("Mina died"))
		    }

	}

}
