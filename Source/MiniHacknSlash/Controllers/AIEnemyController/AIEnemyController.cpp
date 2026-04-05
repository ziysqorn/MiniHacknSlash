// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"
#include "../../Characters/AICharacter/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

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

void AAIEnemyController::SetTarget()
{
	if (GetBlackboardComponent()) {
		if (APlayerController* MainController = GetWorld()->GetFirstPlayerController()) {
			GetBlackboardComponent()->SetValueAsObject(FName("Target"), MainController->GetPawn());
		}
	}
}
