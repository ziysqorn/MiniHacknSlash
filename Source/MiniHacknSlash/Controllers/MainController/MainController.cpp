// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../../Interfaces/CanMove/CanMove.h"
#include "../../Interfaces/CanDodge/CanDodge.h"

void AMainController::BeginPlay()
{
	Super::BeginPlay();
}

void AMainController::SetupInputComponent()
{
	Super::SetupInputComponent();


	if (UEnhancedInputComponent* EIComponent = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		if (IsValid(IA_Move)) EIComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainController::MoveTriggered);
		if (IsValid(IA_Look)) EIComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &AMainController::Look);
		if (IsValid(IA_Dodge)) EIComponent->BindAction(IA_Dodge, ETriggerEvent::Triggered, this, &AMainController::DodgeTriggered);
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
		FVector2D MoveDir = value.Get<FVector2D>();
		CanMove->Move(MoveDir);
	}
}

void AMainController::DodgeTriggered()
{
	if (ICanDodge* CanDodge = Cast<ICanDodge>(this->GetPawn())) {
		CanDodge->Dodge();
	}
}

void AMainController::Look(const FInputActionValue& value)
{
	FVector lookDirectionVal = value.Get<FVector>();
	AddYawInput(lookDirectionVal.X);
	AddPitchInput(lookDirectionVal.Y);
}