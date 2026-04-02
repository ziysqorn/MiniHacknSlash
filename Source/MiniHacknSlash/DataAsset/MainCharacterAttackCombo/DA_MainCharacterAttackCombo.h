// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_MainCharacterAttackCombo.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UDA_MainCharacterAttackCombo : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AttackComboMap")
	TMap<FString, FString> AttackComboMap;

public:
	UFUNCTION(BlueprintCallable)
	FString GetAttackCombo(const FString& InputString) {
		if (AttackComboMap.Find(InputString)) {
			return *AttackComboMap.Find(InputString);
		}
		return TEXT("");
	}
};
