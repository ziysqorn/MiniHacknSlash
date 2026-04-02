// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API UBTTask_ActivateAbility : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_ActivateAbility();

protected:
	UPROPERTY(EditAnywhere, Category = "AbilitySubclass")
	TSubclassOf<UGameplayAbility> AbilitySubclass;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void SetTaskResultSucceeded(UGameplayAbility* Ability);

private:
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp = nullptr;
};
