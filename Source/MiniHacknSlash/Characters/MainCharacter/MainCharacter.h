// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "MotionWarpingComponent.h"
#include "../../AttributeSet/BaseCharacter/AttrSet_BaseCharacter.h"
#include "../../ActorComponents/CombatComponent/CombatComponent.h"
#include "../../ActorComponents/GameFeelComponent/GameFeelComponent.h"
#include "../BaseCharacter/BaseCharacter.h"
#include "../../Interfaces/CanMove/CanMove.h"
#include "../../Interfaces/CanDodge/CanDodge.h"
#include "../../Interfaces/CanLockTarget/CanLockTarget.h"
#include "../../Interfaces/CanBlock/CanBlock.h"
#include "../../Interfaces/CanAttack/CanAttack.h"
#include "../../Interfaces/CanHeavyAttack/CanHeavyAttack.h"
#include "MainCharacter.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AMainCharacter : public ABaseCharacter, public ICanMove, public ICanDodge, public ICanLockTarget, public ICanBlock, public ICanAttack, public ICanHeavyAttack
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision Box Component")
	TObjectPtr<UBoxComponent> CollisionBoxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MotionWarping")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorComponent | CombatComponent")
	TObjectPtr<UCombatComponent> CombatComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorComponent | CombatComponent")
	TObjectPtr<UGameFeelComponent> GameFeelComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Set")
	TObjectPtr<UAttrSet_BaseCharacter> AttriSet;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Detect Box Extent")
	FVector DetectBoxExtent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | Counter Attack Box Extent")
	FVector CounterAttackBoxExtent;

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

	void HeavyAttack() override;

	void LockTarget() override;

	TWeakObjectPtr<AActor> GetLockedOnTarget() override {
		return LockedOnTarget;
	}

	void SetMovementAfterLockTarget();

	void RotateToLockTarget(float DeltaTime);

	AActor* DetectForCounterAttack();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector GetDesiredDodgeDirection() override;
};
