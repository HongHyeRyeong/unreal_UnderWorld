// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyCharacter.h"
#include "SpawnPoint.h"
#include "Machine.h"
#include "Door.h"
#include "UnderWorldGameMode.generated.h"

UCLASS(minimalapi)
class AUnderWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> StartWidget;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> GameWidget;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "UMG")
	TSubclassOf<UUserWidget> ClearWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* StartSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* BGMSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* RestartSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* ClearSound;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AItemBase>> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> EnemyCharacterClass;

	UPROPERTY()
	int MachineInstallCompleteCount = 0;

public:
	AUnderWorldGameMode();

	UPROPERTY(BlueprintReadWrite)
	int Stage = 0;

	UPROPERTY()
	AUnderWorldCharacter* SurvivorCharacter;

	UPROPERTY()
	AEnemyCharacter* EnemyCharacter;

	UPROPERTY()
	TArray<ASpawnPoint*> EnemySpawnPoints;

	UPROPERTY()
	TArray<AMachine*> Machines;

	UPROPERTY()
	TArray<ADoor*> Doors;

	UPROPERTY()
	ASpawnPoint* DoorReachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UAudioComponent* BGMAudioComponent;

	UFUNCTION()
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UFUNCTION()
	void StartGame(int StartStage);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void ClearGame();

	UFUNCTION()
	void SpawnItem();

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void TeleportEnemy();

	UFUNCTION()
	void CompleteMachineInstall();

	UFUNCTION()
	void SetDoorReachPoint();
};