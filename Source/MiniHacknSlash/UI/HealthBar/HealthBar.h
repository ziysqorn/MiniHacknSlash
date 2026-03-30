// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UProgressBar> PBar_Health;

public:
	void BindHealthBar(UObject* inObj, const FName& inFunc) {
		if (IsValid(PBar_Health)) {
			PBar_Health->PercentDelegate.BindUFunction(inObj, inFunc);
		}
	}
};
