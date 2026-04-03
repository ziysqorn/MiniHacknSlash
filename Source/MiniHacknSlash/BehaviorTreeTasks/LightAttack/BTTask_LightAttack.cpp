// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LightAttack.h"
#include "AIController.h"
#include "../../Interfaces/CanAttack/CanAttack.h"

UBTTask_LightAttack::UBTTask_LightAttack()
{
	NodeName = TEXT("Perfrom Light Attack");
}

EBTNodeResult::Type UBTTask_LightAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner()) {
		if (ICanAttack* CanAttack = Controller->GetPawn<ICanAttack>()) {
			CanAttack->LightAttack();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
