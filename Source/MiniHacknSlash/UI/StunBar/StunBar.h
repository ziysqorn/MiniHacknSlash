// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StunBar.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UStunBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> PBar_Stun;

public:
	void BindHealthBar(UObject* inObj, const FName& inFunc) {
		if (IsValid(PBar_Stun)) {
			PBar_Stun->PercentDelegate.BindUFunction(inObj, inFunc);
		}
	}
};
