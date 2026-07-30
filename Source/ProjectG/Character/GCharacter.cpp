#include "GCharacter.h"

#include "Camera/CameraComponent.h"
#include "Component/GInteractionComponent.h"
#include "Component/GInventoryComponent.h"
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
}

void AGCharacter::BeginPlay()
{
	Super::BeginPlay();


}
