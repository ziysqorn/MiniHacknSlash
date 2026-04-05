// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class MINIHACKNSLASH_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIEnemyController();

	void SetTarget();
protected:
	void OnPossess(APawn* inPawn) override;
};
