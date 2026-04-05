// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyStatsWidgetComponent.generated.h"

/**
 * 
 */
struct FOnAttributeChangeData;

UCLASS()
class MINIHACKNSLASH_API UEnemyStatsWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnStunBarChanged(const FOnAttributeChangeData& Data);
};
