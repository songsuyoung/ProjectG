#include "Component/GQuestComponent.h"

#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GQuestRow.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"

void UGQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	GEVENT_ADD(this, GGameplayTags::EventTag_Quest_Accept, this);
}

void UGQuestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Quest_Accept, this);
}

void UGQuestComponent::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	if (Tag == GGameplayTags::EventTag_Quest_Accept)
	{
		FGQuestMessage* QuestMsg = static_cast<FGQuestMessage*>(Message);
		if (nullptr != QuestMsg)
		{
			AcceptQuest(QuestMsg->QuestID);
		}
	}
}

FName UGQuestComponent::GetSuccessDialogueForNPC(FName NPCID, FName& OutQuestID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	for (FGQuestEntry& Entry : Quests)
	{
		if (Entry.State != EGQuestState::ReadyToComplete)
		{
			continue;
		}

		FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, Entry.QuestID);
		if (nullptr == Row || Row->NPCID != NPCID)
		{
			continue;
		}

		OutQuestID = Entry.QuestID;
		return Row->SuccessDialogueID;
	}
	OutQuestID = NAME_None;
	return NAME_None;
}

FName UGQuestComponent::GetPreDialogueForNPC(FName NPCID, FName& OutQuestID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	for (FGQuestEntry& Entry : Quests)
	{
		if (Entry.State != EGQuestState::Available)
		{
			continue;
		}

		FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, Entry.QuestID);
		if (nullptr == Row || Row->NPCID != NPCID)
		{
			continue;
		}

		OutQuestID = Entry.QuestID;
		return Row->PreDialogueID;
	}
	OutQuestID = NAME_None;
	return NAME_None;
}

void UGQuestComponent::AcceptQuest(FName QuestID)
{
	FGQuestEntry* Entry = FindEntry(QuestID);
	if (nullptr == Entry || Entry->State != EGQuestState::Available)
	{
		return;
	}

	Entry->State = EGQuestState::Active;
}

void UGQuestComponent::FinishObjective(FName QuestID)
{
	FGQuestEntry* Entry = FindEntry(QuestID);
	if (nullptr == Entry || Entry->State != EGQuestState::Active)
	{
		return;
	}

	Entry->State = EGQuestState::ReadyToComplete;
}

void UGQuestComponent::CompleteQuest(FName QuestID)
{
	FGQuestEntry* Entry = FindEntry(QuestID);
	if (nullptr == Entry || Entry->State != EGQuestState::ReadyToComplete)
	{
		return;
	}

	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);

	FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, QuestID);
	const FName NextQuestID = (nullptr != Row) ? Row->NextQuestID : NAME_None;

	Quests.RemoveAll([&QuestID](const FGQuestEntry& E) { return E.QuestID == QuestID; });

	if (false == NextQuestID.IsNone())
	{
		FGQuestEntry NewEntry;
		NewEntry.QuestID = NextQuestID;
		NewEntry.State = EGQuestState::Available;
		Quests.Add(NewEntry);
	}
}

void UGQuestComponent::InitQuests(const TArray<FName>& QuestIDs)
{
	Quests.Empty();
	for (const FName& ID : QuestIDs)
	{
		FGQuestEntry Entry;
		Entry.QuestID = ID;
		Entry.State = EGQuestState::Available;
		Quests.Add(Entry);
	}
}

FGQuestEntry* UGQuestComponent::FindEntry(FName QuestID)
{
	return Quests.FindByPredicate([QuestID](const FGQuestEntry& E) { return E.QuestID == QuestID; });
}
