// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UnderWorldGameInstance.generated.h"

UCLASS()
class UNDERWORLD_API UUnderWorldGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int Stage = 0;

	UPROPERTY()
	int LastStage = 3;

	FORCEINLINE bool IsFinalStage() { return Stage == LastStage; }
};
