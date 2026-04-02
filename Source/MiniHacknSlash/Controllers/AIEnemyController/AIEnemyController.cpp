// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "../../Characters/AICharacter/AICharacter.h"

AAIEnemyController::AAIEnemyController()
{
}

void AAIEnemyController::OnPossess(APawn* inPawn)
{
	Super::OnPossess(inPawn);

	if (AAICharacter* AICharacter = Cast<AAICharacter>(inPawn)) {
		if (UBehaviorTree* Tree = AICharacter->GetBehaviorTree()) {
			UBlackboardComponent* temp = nullptr;
			UseBlackboard(Tree->GetBlackboardAsset(), temp);
			Blackboard = temp;
			RunBehaviorTree(Tree);
		}
	}
}
