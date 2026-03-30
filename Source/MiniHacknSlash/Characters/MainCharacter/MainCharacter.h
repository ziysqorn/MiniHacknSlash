// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../../Interfaces/CanMove/CanMove.h"
#include "../../Interfaces/CanDodge/CanDodge.h"
#include "AbilitySystemInterface.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AMainCharacter : public ABaseCharacter, public ICanMove, public ICanDodge, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SpringArm")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystemComponent")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FVector2D& MoveDir) override;

	void Dodge() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return AbilitySystemComp;
	}
};
