#include "Component/GHeroComponent.h"

// UE 5.
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GInteractionComponent.h"
#include "Character/GCharacter.h"
#include "GameFramework/Character.h"

UGHeroComponent::UGHeroComponent()
{
}

void UGHeroComponent::SetupInputComponent()
{
		// 없으면 캐스팅 시도를 해본다.
	PlayerControllerRef = Cast<APlayerController>(GetOwner());
	
	if (PlayerControllerRef.IsValid())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerRef->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerControllerRef->InputComponent);

		if (IsValid(EnhancedInputComponent))
		{
			EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
			EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
			
			EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Started, this, &ThisClass::OnInteractStarted);
			EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Completed, this, &ThisClass::OnInteractEnded);
		}
	}
}

void UGHeroComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Initalize();
}

void UGHeroComponent::Initalize()
{
	if (false == CharacterRef.IsValid())
	{
		if (PlayerControllerRef.IsValid())
		{
			CharacterRef = Cast<AGCharacter>(PlayerControllerRef->GetCharacter());
		}
	}
}

void UGHeroComponent::Move(const FInputActionValue& Value)
{
	if (false == PlayerControllerRef.IsValid() || false == CharacterRef.IsValid())
	{
		return;
	}
	
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = PlayerControllerRef->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	CharacterRef->AddMovementInput(ForwardDirection, MovementVector.Y);
	CharacterRef->AddMovementInput(RightDirection, MovementVector.X);
}

void UGHeroComponent::Look(const FInputActionValue& Value)
{
	if (PlayerControllerRef.IsValid())
	{
		FVector2D LookAxisVector = Value.Get<FVector2D>();
		PlayerControllerRef->AddYawInput(LookAxisVector.X);
		PlayerControllerRef->AddPitchInput(LookAxisVector.Y);
	}
}

void UGHeroComponent::OnInteractStarted()
{
	if (CharacterRef.IsValid())
	{
		UGInteractionComponent* InteractionComponent = CharacterRef->GetInteractionComponent();
		
		if (IsValid(InteractionComponent))
		{
			InteractionComponent->OnInteractStarted();
		}
	}
}

void UGHeroComponent::OnInteractEnded()
{
	if (CharacterRef.IsValid())
	{
		UGInteractionComponent* InteractionComponent = CharacterRef->GetInteractionComponent();

		if (IsValid(InteractionComponent))
		{
			InteractionComponent->OnInteractEnded();
		}
	}
}


