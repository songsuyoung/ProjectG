#include "Component/GQuestComponent.h"

#include "Data/GGameMacro.h"
#include "Data/GGameplayTags.h"
#include "Data/GMessage.h"
#include "Data/GQuestRow.h"
#include "Data/Condition/GCondition.h"
#include "System/GDataManager.h"
#include "System/GEventManager.h"
#include "Engine/DataTable.h"
#include "System/GConditionManager.h"

void UGQuestComponent::BeginPlay()
{
	Super::BeginPlay();
	
	GEVENT_ADD(this, GGameplayTags::EventTag_Open_DataTable, this);
	GEVENT_ADD(this, GGameplayTags::EventTag_Dialogue_SelectChoice, this);
}

void UGQuestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Open_DataTable, this);
	GEVENT_REMOVE(this, GGameplayTags::EventTag_Dialogue_SelectChoice, this);
}

void UGQuestComponent::OnMessage(FGameplayTag Tag, FGMessage* Message)
{
	FGChoiceMessage* ChoiceMsg = static_cast<FGChoiceMessage*>(Message);
	
	if (nullptr != ChoiceMsg)
	{
		if (ChoiceMsg->ChoiceTag == GGameplayTags::EventTag_Quest_Accept)
		{
			AcceptQuest(PendingQID);
			PendingQID = NAME_None;
		}
		
		return;
	}
	
	if (Tag == GGameplayTags::EventTag_Open_DataTable)
	{
		InitQuest();
	}
}

void UGQuestComponent::CheckQuest(const FName& NPCID)
{
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	
	TArray<FName> FinishedQuests;
   	for (FName ActiveQuestID : ActiveQuestIDs)
	{
		FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, ActiveQuestID);
		
		if (nullptr != Row)
		{
			if (Row->NPCID != NPCID)
			{
				continue;
			}
			
			bool bIsSatisfied = true;
			UGConditionManager* ConditionManager = UGConditionManager::Get(this);
			check(ConditionManager);
				
			for (const FName& Condition : Row->Conditions)
			{
				if (false == ConditionManager->IsSatisfied(Condition, GetOwner()))
				{
					bIsSatisfied = false;
					break;
				}
			}
		
			if (bIsSatisfied)
			{
				FinishedQuests.Add(ActiveQuestID);
			}
		}
	}
	
	for (const FName& FinishedQuestID : FinishedQuests)
	{
		FinishObjective(FinishedQuestID);
	}
}

FName UGQuestComponent::GetDialogueForNPC(FName NPCID)
{
   	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	
	FGDialogueEntry BestQuest;
	
	for (const FGQuestEntry& Entry : Quests)
	{
		FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, Entry.QuestID);
		if (nullptr != Row && Row->NPCID == NPCID)
		{
			if (Entry.State > BestQuest.State)
			{
				FName* DID = Row->DialogueID.Find(Entry.State);
				
				if (DID != nullptr)
				{
					BestQuest ={Entry.QuestID, Entry.State, *DID}; 
				}
			}
		}
	}
	
	PendingQID = BestQuest.QuestID;
	return BestQuest.DialogueID;
}

EGQuestState UGQuestComponent::GetQuestState(FName QuestID) const
{
	const FGQuestEntry* Entry = FindEntry(QuestID);
	return nullptr != Entry ? Entry->State : EGQuestState::None;
}

bool UGQuestComponent::CanAcceptQuest(FName QuestID)
{
	const EGQuestState State = GetQuestState(QuestID);
	if (State != EGQuestState::None && State != EGQuestState::Available)
	{
		return false;
	}

	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	const FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, QuestID);
	if (nullptr == Row)
	{
		return false;
	}

	return Row->PrerequisiteQID.IsNone() || GetQuestState(Row->PrerequisiteQID) == EGQuestState::Completed;
}

void UGQuestComponent::OnNPCDialogueCompleted(FName NPCID)
{
	UGDataManager* DataManager = UGDataManager::Get(GetOwner());      
	
	check(DataManager);
	
	for (FGQuestEntry& Entry : Quests)
	{
		if (Entry.State != EGQuestState::ReadyToComplete) continue;
		FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, Entry.QuestID);
		if (nullptr != Row && Row->NPCID == NPCID)
		{
			CompleteQuest(Entry.QuestID);
			return;
		}
	}
}

bool UGQuestComponent::AcceptQuest(FName QuestID)
{
	FGQuestEntry* Entry = FindEntry(QuestID);
	if (false == CanAcceptQuest(QuestID))
	{
		return false;
	}

	if (nullptr == Entry)
	{
		FGQuestEntry NewEntry;
		NewEntry.QuestID = QuestID;
		NewEntry.State = EGQuestState::Active;
		Quests.Add(NewEntry);
	}
	else
	{
		Entry->State = EGQuestState::Active;
	}

	ActiveQuestIDs.AddUnique(QuestID);
	return true;
}

void UGQuestComponent::FinishObjective(FName QuestID)
{
	FGQuestEntry* Entry = FindEntry(QuestID);
	if (nullptr == Entry || Entry->State != EGQuestState::Active)
	{
		return;
	}

	Entry->State = EGQuestState::ReadyToComplete;
	ActiveQuestIDs.Remove(QuestID);
}

void UGQuestComponent::InitQuest()
{
	// SaveData가 없을 때 
	UGDataManager* DataManager = UGDataManager::Get(this);
	
	check(DataManager);
	
	UDataTable* QuestTable = DataManager->GetDataTable(EGDataTableType::Quest);
	if (IsValid(QuestTable))
	{
		TArray<FGQuestRow*> QuestRows;
		
		QuestTable->GetAllRows(TEXT(""), QuestRows);
		
		for (FGQuestRow* QuestRow : QuestRows)
		{
			if (false == CanAcceptQuest(QuestRow->GetID()))
			{
				continue;
			}
			
			if (nullptr == FindEntry(QuestRow->GetID()))
			{
				Quests.Add({QuestRow->GetID(), EGQuestState::Available});
			}
		}
	}
	
	// SaveData가 있다면, Quests 에서 Active 상태인 퀘스트 ID를 ActiveQuestIDs 보관
}

bool UGQuestComponent::CompleteQuest(FName QuestID)
{
	FGQuestEntry* Entry = FindEntry(QuestID);
	if (nullptr == Entry || Entry->State != EGQuestState::ReadyToComplete)
	{
		return false;
	}
	
	UGDataManager* DataManager = UGDataManager::Get(this);
	check(DataManager);
	const FGQuestRow* Row = DataManager->GetDataTableRow<FGQuestRow>(EGDataTableType::Quest, QuestID);
	if (nullptr == Row)
	{
		return false;
	}
	
	if (false == Row->NextQID.IsNone())
	{
		Quests.Add({Row->NextQID, EGQuestState::Available});
	}

	Entry->State = EGQuestState::Completed;
	return true;
}

FGQuestEntry* UGQuestComponent::FindEntry(FName QuestID)
{
	return Quests.FindByPredicate([QuestID](const FGQuestEntry& E){ return E.QuestID == QuestID; });;
}

const FGQuestEntry* UGQuestComponent::FindEntry(FName QuestID) const
{
	return Quests.FindByPredicate([QuestID](const FGQuestEntry& E){ return E.QuestID == QuestID; });;
}
