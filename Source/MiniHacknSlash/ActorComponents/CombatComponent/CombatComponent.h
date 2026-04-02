// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../DataAsset/MainCharacterAttackCombo/DA_MainCharacterAttackCombo.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MINIHACKNSLASH_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	UPROPERTY()
	TArray<FString> AttackInputBuffer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Need set | Attack Combo Data")
	TObjectPtr<UDA_MainCharacterAttackCombo> DA_AttackCombo;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void GetAttackInputBuffer(TArray<FString>& OutBuffer) {
		OutBuffer = AttackInputBuffer;
	}

	UFUNCTION(BlueprintCallable)
	void AddAttackInputToBuffer(const FString& Input) {
		if (AttackInputBuffer.Num() == 10) {
			AttackInputBuffer.RemoveAt(0, EAllowShrinking::No);
		}
		AttackInputBuffer.Add(Input);
	}

	UFUNCTION(BlueprintCallable)
	void ClearAttackInputBuffer() {
		AttackInputBuffer.Empty(10);
	}

	UFUNCTION(BlueprintCallable)
	FString ChooseAttackCombo() {
		if (IsValid(DA_AttackCombo)) {
			FString AttackInputString;
			for (auto AttackInput : AttackInputBuffer) {
				AttackInputString = AttackInputString.Append(AttackInput);
			}
			return DA_AttackCombo->GetAttackCombo(AttackInputString);
		}
		return TEXT("");
	}
};
