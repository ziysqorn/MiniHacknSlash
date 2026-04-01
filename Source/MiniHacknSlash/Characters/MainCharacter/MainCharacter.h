// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "../../Interfaces/CanMove/CanMove.h"
#include "../../Interfaces/CanDodge/CanDodge.h"
#include "../../Interfaces/CanLockTarget/CanLockTarget.h"
#include "../../Interfaces/CanBlock/CanBlock.h"
#include "../../Interfaces/CanAttack/CanAttack.h"
#include "../../DataAsset/MainCharacterAbilities/DA_MainCharacterAbilities.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AMainCharacter : public ABaseCharacter, public ICanMove, public ICanDodge, public ICanLockTarget, public ICanBlock, public ICanAttack
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | GameplayAbilityDataAsset")
	TObjectPtr<UDA_MainCharacterAbilities> DA_GameplayAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Detect Box Extent")
	FVector DetectBoxExtent;

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<AActor> LockedOnTarget = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GrantCharacterAbilities();

	void Move(const FVector& MoveDir) override;

	void Dodge() override;

	void Block() override;

	void EndBlock() override;

	void LightAttack() override;

	void LockTarget() override;

	TWeakObjectPtr<AActor> GetLockedOnTarget() override {
		return LockedOnTarget;
	}

	void SetMovementAfterLockTarget();

	void RotateToLockTarget(float DeltaTime);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetDesiredDodgeDirection() override;
};
