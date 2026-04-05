// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "../../Interfaces/CanBindHealthUI/CanBindHealthUI.h"
#include "../../Interfaces/CanBindStunUI/CanBindStunUI.h"
#include "EnemyStatsUI.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UEnemyStatsUI : public UUserWidget, public ICanBindHealthUI, public ICanBindStunUI
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> PBar_Health;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> PBar_Stun;

	void NativeDestruct() override;

public:
	void BindHealthBar(UObject* inObj, const FName& inFunc) override {
		if (IsValid(PBar_Health)) {
			PBar_Health->PercentDelegate.BindUFunction(inObj, inFunc);
		}
	}

	void SetHealthPercentage(float Percentage) override {
		if (IsValid(PBar_Health)) {
			PBar_Health->SetPercent(Percentage);
		}
	}

	void BindStunBar(UObject* inObj, const FName& inFunc) override {
		if (IsValid(PBar_Stun)) {
			PBar_Stun->PercentDelegate.BindUFunction(inObj, inFunc);
		}
	}

	void SetStunBarPercentage(float Percentage) override {
		if (IsValid(PBar_Stun)) {
			PBar_Stun->SetPercent(Percentage);
		}
	}
};
