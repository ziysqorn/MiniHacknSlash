// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatsUI.h"

void UEnemyStatsUI::NativeDestruct()
{
	if (IsValid(PBar_Health)) PBar_Health->PercentDelegate.Clear();
	if (IsValid(PBar_Stun)) PBar_Stun->PercentDelegate.Clear();

	Super::NativeDestruct();
}
