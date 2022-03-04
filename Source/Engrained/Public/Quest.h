// The game and source code belongs to Team 7 Programming team

#pragma once

#include "CoreMinimal.h"
#include "SM_State.h"
#include "Components/ActorComponent.h"
#include "Engine/DataAsset.h"
#include "Quest.generated.h"

class USM_State;
class USM_Branch;
class USM_InputAtom;

UENUM()
enum class EQuestCompletion : uint8
{
	EQC_NotStarted,
	EQC_Started,
	EQC_Succeded,
	EQC_Failed
};

/**
 * 
 */
UCLASS()
class ENGRAINED_API UQuest : public UDataAsset
{
	GENERATED_BODY()
	
public:
	//	The name of the quest
	UPROPERTY(EditAnywhere)
	FText QuestName;

	//	If this machine accepts our QuestActivites log, the quest is successful
	UPROPERTY(EditAnywhere)
	USM_State* QuestStateMachine;
		
	// If true, the InputList is a blacklist. Otherwise, it's a whitelist
	UPROPERTY(EditAnywhere)
	uint32 bInputBlackList : 1;

	//	The blacklist/whitelist (depending on bBlackList) used to filter InputAtoms this Quest recognizes
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> InputList;
};

USTRUCT()
struct FQuestInProgress
{
	GENERATED_BODY()

	//	Quest data asset
	UPROPERTY(EditAnywhere)
	const UQuest* Quest;

	//	Current progress in the quest
	UPROPERTY(EditAnywhere)
	EQuestCompletion QuestProgress;

protected:
	//	All input for this quest. Filtered by the quest's blacklist/whitelist
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> QuestActivites;

public:

	//	Returns true if the quest was completed (success or failure) by the new activity
	bool UpdateQuest(const UObject* ObjectRef, USM_InputAtom* QuestActivity)
	{
		//	Only log activity to valig, in-progress quests. Check the blacklist/whitelist before logging
		if (Quest && (QuestProgress == EQuestCompletion::EQC_Started) 
			&& (Quest->bInputBlackList != Quest->InputList.Contains(QuestActivity)))
		{
			FStateMachineResult QuestResult;
			QuestActivites.Add(QuestActivity);
			QuestResult = Quest->QuestStateMachine->RunState(ObjectRef, QuestActivites);
			switch (QuestResult.CompletionType)
			{
			case EStateMachineCompletionType::Accepted:
				QuestProgress = EQuestCompletion::EQC_Succeded;
				return true;
			case EStateMachineCompletionType::Rejected:
				QuestProgress = EQuestCompletion::EQC_Failed;
				return true;
				//	case NotAccepted: // Still in progress. No update needed. 
			}
		}
		return false;
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ENGRAINED_API UQuestStatus : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestStatus();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);

	/**	Add to our quest activity log!This also automatically checks to if any
	* unfinished quests are now complete */
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void UpdateQuests(USM_InputAtom* QuestActivity);

	UFUNCTION(BlueprintCallable, Category = "Quests")
	void QuestLog();

protected:
	//	The master list of all quest-related things we've done
	UPROPERTY(EditAnywhere)
	TArray<USM_InputAtom*> QuestActivities;

	//	The list of quests in our current game or area
	UPROPERTY(EditAnywhere)
	TArray<FQuestInProgress> QuestList;

};