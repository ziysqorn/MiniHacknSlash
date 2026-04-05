// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../ActorComponents/CombatComponent/CombatComponent.h"
#include "../../Interfaces/CanMove/CanMove.h"
#include "../../Interfaces/CanDodge/CanDodge.h"
#include "../../Interfaces/CanLockTarget/CanLockTarget.h"
#include "../../Interfaces/CanBlock/CanBlock.h"
#include "../../Interfaces/CanAttack/CanAttack.h"
#include "../../Interfaces/CanHeavyAttack/CanHeavyAttack.h"


AMainController::AMainController()
{
	HUDComp = CreateDefaultSubobject<UHUDComponent>("HUDComp");
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();

	SetupMappingContext();
}

void AMainController::SetupInputComponent()
{
	Super::SetupInputComponent();


	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		if (IsValid(IA_Move)) EIComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainController::MoveTriggered);
		if (IsValid(IA_Look)) EIComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainController::Look);
		if (IsValid(IA_Dodge)) EIComponent->BindAction(IA_Dodge, ETriggerEvent::Triggered, this, &AMainController::DodgeTriggered);
		if (IsValid(IA_LockTarget)) EIComponent->BindAction(IA_LockTarget, ETriggerEvent::Triggered, this, &AMainController::LockTargetTriggered);
		if (IsValid(IA_Block)) EIComponent->BindAction(IA_Block, ETriggerEvent::Triggered, this, &AMainController::BlockTriggered);
		if (IsValid(IA_Block)) EIComponent->BindAction(IA_Block, ETriggerEvent::Completed, this, &AMainController::BlockTriggered);
		if (IsValid(IA_Melee_LightAttack)) EIComponent->BindAction(IA_Melee_LightAttack, ETriggerEvent::Triggered, this, &AMainController::LightAttackTriggered);
		if (IsValid(IA_Melee_HeavyAttack)) EIComponent->BindAction(IA_Melee_HeavyAttack, ETriggerEvent::Triggered, this, &AMainController::HeavyAttackTriggered);
	}
}

void AMainController::SetupMappingContext()
{
	if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer())) {
		EISubsystem->AddMappingContext(IMC_MainCharacter, 1);
	}
}

void AMainController::MoveTriggered(const FInputActionValue& value)
{
	if (ICanMove* CanMove = Cast<ICanMove>(this->GetPawn())) {
		FVector MoveDir = value.Get<FVector>();
		CanMove->Move(MoveDir);
	}
}

void AMainController::DodgeTriggered()
{
	if (ICanDodge* CanDodge = Cast<ICanDodge>(this->GetPawn())) {
		CanDodge->Dodge();
	}
}

void AMainController::LockTargetTriggered()
{
	if (ICanLockTarget* CanLockTarget = Cast<ICanLockTarget>(this->GetPawn())) {
		CanLockTarget->LockTarget();
	}
}

void AMainController::BlockTriggered()
{
	if (ICanBlock* CanBlock = Cast<ICanBlock>(this->GetPawn())) {
		CanBlock->Block();
	}
}

void AMainController::BlockEnd()
{
	if (ICanBlock* CanBlock = Cast<ICanBlock>(this->GetPawn())) {
		CanBlock->EndBlock();
	}
}

void AMainController::LightAttackTriggered()
{
	AddSavedInputToBuffer(TEXT("L"));
	if (ICanAttack* CanAttack = Cast<ICanAttack>(this->GetPawn())) {
		CanAttack->LightAttack();
	}
}

void AMainController::HeavyAttackTriggered()
{
	//AddSavedInputToBuffer(TEXT("H"));
	if (ICanHeavyAttack* CanHeavyAttack = Cast<ICanHeavyAttack>(this->GetPawn())) {
		CanHeavyAttack->HeavyAttack();
	}
}

FVector AMainController::GetMoveInputValue()
{
	if (UEnhancedInputLocalPlayerSubsystem* EISubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer())) {
		if (UEnhancedPlayerInput* EnhancedPlayerInput = EISubsystem->GetPlayerInput()) {
			if (IsValid(IA_Move)) {
				const FInputActionValue ActionValue = EnhancedPlayerInput->GetActionValue(IA_Move);
				return ActionValue.Get<FVector>();
			}
		}
	}
	return FVector();
}

void AMainController::GetSavedInputBuffer_Implementation(TArray<FString>& OutBuffer)
{
	OutBuffer = SavedInputBuffer;
}

void AMainController::ClearSavedInputBuffer_Implementation()
{
	SavedInputBuffer.Empty(10);
}

void AMainController::Look(const FInputActionValue& value)
{
	bool bCanLook = true;
	if (ICanLockTarget* CanLockTarget = Cast<ICanLockTarget>(this->GetPawn())) {
		if (CanLockTarget->GetLockedOnTarget().IsValid()) {
			bCanLook = false;
		}
	}
	if (bCanLook) {
		FVector lookDirectionVal = value.Get<FVector>();
		AddYawInput(lookDirectionVal.X);
		AddPitchInput(lookDirectionVal.Y);
	}
}