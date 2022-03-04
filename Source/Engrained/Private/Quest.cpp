// The game and source code belongs to Team 7 Programming team

//#include "Engrained.h"
#include "Quest.h"

UQuestStatus::UQuestStatus()
{
	//bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestStatus::BeginPlay()
{
	Super::BeginPlay();
}

void UQuestStatus::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UQuestStatus::UpdateQuests(USM_InputAtom* QuestActivity)
{
	//	Update the master list of everything we've ever done
	QuestActivities.Add(QuestActivity);

	//	Update individual quests (if they care about this activity) and see if they are complete
	for (int32 i = QuestList.Num() - 1; i >= 0; i--)
	{
		if (QuestList[i].UpdateQuest(this, QuestActivity))
		{
			UE_LOG(LogTemp, Warning, TEXT("Quest \"%s\" completed: %s"),
				*QuestList[i].Quest->QuestName.ToString(),
				(QuestList[i].QuestProgress == EQuestCompletion::EQC_Succeded) ? TEXT("Success") : TEXT("Failure"));
		} 
	}
}

void UQuestStatus::QuestLog()
{
	for (int32 i = QuestList.Num() - 1; i >= 0; i--)
	{
		for (int32 j = QuestActivities.Num() - 1; j >= 0; j--)
		{
			UE_LOG(LogTemp, Warning, TEXT("Quest: %s : Activity: %s"), *QuestList[i].Quest->QuestName.ToString(),
				*QuestActivities[j]->Description.ToString());
		}
	}
}
