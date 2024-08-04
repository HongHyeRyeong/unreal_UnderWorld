// Fill out your copyright notice in the Description page of Project Settings.


#include "UnderWorldGameInstance.h"

void UUnderWorldGameInstance::Init()
{
	Super::Init();

	Stage = 0;
	LastStage = 3;

	for (int i = 0; i < (int)EItemType::MAX; ++i)
	{
		HaveItemMap.Add((EItemType)i, 0);
		PutItemMap.Add((EItemType)i, 0);
	}
}