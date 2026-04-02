// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Abilities/GameplayAbility.h"
#include "DA_GameplayAbilities.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UDA_GameplayAbilities : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayAbilitySubclass")
	TMap<FString, TSubclassOf<UGameplayAbility>> GameplayAbilitySubclassMap;

public:
	TSubclassOf<UGameplayAbility> GetGameplayAbilitySubclass(const FString& AbilityName) {
		if (GameplayAbilitySubclassMap.Find(AbilityName)) {
			return *GameplayAbilitySubclassMap.Find(AbilityName);
		}
		return TSubclassOf<UGameplayAbility>();
	}
};
