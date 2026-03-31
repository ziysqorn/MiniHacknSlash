// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MainController.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AMainController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputAction | Move")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputAction | Look")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputAction | Dodge")
	TObjectPtr<UInputAction> IA_Dodge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputAction | Lock Target")
	TObjectPtr<UInputAction> IA_LockTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputAction | Light Attack")
	TObjectPtr<UInputAction> IA_LightAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputAction | Block")
	TObjectPtr<UInputAction> IA_Block;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | InputMappingContext")
	TObjectPtr<UInputMappingContext> IMC_MainCharacter;

	void SetupMappingContext();

	void SetupInputComponent() override;

	void BeginPlay() override;

	void MoveTriggered(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void DodgeTriggered();

	void LockTargetTriggered();
};
