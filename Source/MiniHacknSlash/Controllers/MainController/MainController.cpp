// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../Interfaces/CanMove/CanMove.h"
#include "../../Interfaces/CanDodge/CanDodge.h"
#include "../../Interfaces/CanLockTarget/CanLockTarget.h"

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