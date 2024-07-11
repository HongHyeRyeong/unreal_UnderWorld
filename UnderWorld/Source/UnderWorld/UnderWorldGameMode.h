// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Machine.h"
#include "EnemyCharacter.h"
#include "UnderWorldGameMode.generated.h"

UCLASS(minimalapi)
class AUnderWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:

	UPROPERTY()
	int MachineInstallCompleteCount = 0;

public:
	AUnderWorldGameMode();

	UPROPERTY(BlueprintReadWrite)
	int Stage = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> EnemyCharacterClass;

	UPROPERTY()
	AEnemyCharacter* EnemyCharacter;

	UPROPERTY()
	TArray<AMachine*> Machines;

	UFUNCTION(BlueprintCallable)
	void SpawnEnemyByMachine();

	UFUNCTION(BlueprintCallable)
	void TeleportEnemy();

	UFUNCTION()
	void TeleportEnemyByMachine();

	UFUNCTION(BlueprintCallable)
	void CompleteMachineInstall();
};