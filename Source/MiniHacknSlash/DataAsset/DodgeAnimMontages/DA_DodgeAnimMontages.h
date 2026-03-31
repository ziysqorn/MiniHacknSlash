// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "DA_DodgeAnimMontages.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UDA_DodgeAnimMontages : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Need set | AnimMontagesMap")
	TMap<FString, TObjectPtr<UAnimMontage>> DodgeAnimMontagesMap;

	UPROPERTY(EditDefaultsOnly, Category = "Need set | Movement Input Dodge Map")
	TMap<FVector, FString> MovementInputDodgeMap;

public:
	TObjectPtr<UAnimMontage> GetDodgeMontage(const FString& MontageName) {
		if (DodgeAnimMontagesMap.Find(MontageName)) {
			return *DodgeAnimMontagesMap.Find(MontageName);
		}
		return nullptr;
	}

	TObjectPtr<UAnimMontage> GetDodgeMontage(const FVector& LastInputMovementVector) {
		if (MovementInputDodgeMap.Find(LastInputMovementVector)) {
			FString MontageName = *MovementInputDodgeMap.Find(LastInputMovementVector);
			if (DodgeAnimMontagesMap.Find(MontageName)) {
				return *DodgeAnimMontagesMap.Find(MontageName);
			}
		}
		return nullptr;
	}
};
