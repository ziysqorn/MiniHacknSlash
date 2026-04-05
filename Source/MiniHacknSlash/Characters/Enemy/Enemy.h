// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AICharacter/AICharacter.h"
#include "Components/BoxComponent.h"
#include "../../CustomComponents/EnemyStatsWidgetComponent/EnemyStatsWidgetComponent.h"
#include "../../AttributeSet/BaseCharacter/AttrSet_BaseCharacter.h"
#include "../../ActorComponents/CombatComponent/CombatComponent.h"
#include "../../Interfaces/CanAttack/CanAttack.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AEnemy : public AAICharacter, public ICanAttack
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute Set")
	TObjectPtr<UAttrSet_BaseCharacter> AttriSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Collision Box Component")
	TObjectPtr<UBoxComponent> CollisionBoxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Component")
	TObjectPtr<UEnemyStatsWidgetComponent> StatsWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActorComponent | CombatComponent")
	TObjectPtr<UCombatComponent> CombatComp;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void BillboardingWidgetCompByClient();

	void LightAttack() override;
};
