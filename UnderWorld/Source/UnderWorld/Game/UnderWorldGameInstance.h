// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SurvivorCharacter.h"
#include "UnderWorldGameInstance.generated.h"

UCLASS()
class UNDERWORLD_API UUnderWorldGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadOnly)
	int Stage = 0;

	UPROPERTY()
	int LastStage = 3;

	UPROPERTY()
	TMap<EItemType, int32> HaveItemMap;

	UPROPERTY()
	TMap<EItemType, int32> PutItemMap;

	FORCEINLINE bool IsFinalStage() { return Stage == LastStage; }
};
