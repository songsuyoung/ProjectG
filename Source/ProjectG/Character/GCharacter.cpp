#include "GCharacter.h"

#include "Camera/CameraComponent.h"
#include "Component/GInteractionComponent.h"
#include "Component/GInventoryComponent.h"
#include "Component/GQuestComponent.h"
#include "GameFramework/SpringArmComponent.h"

AGCharacter::AGCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; 
	CameraBoom->bUsePawnControlRotation = true; 

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;
	
	InteractionComponent = CreateDefaultSubobject<UGInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<UGInventoryComponent>(TEXT("InventoryComponent"));
	QuestComponent = CreateDefaultSubobject<UGQuestComponent>(TEXT("QuestComponent"));
}

void AGCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(QuestComponent))
	{
		// 플레이어의 시작을 의미
		QuestComponent->InitQuests({FName("0")});
		QuestComponent->AcceptQuest(FName("0"));
		QuestComponent->FinishObjective(FName("0"));
	}
}
