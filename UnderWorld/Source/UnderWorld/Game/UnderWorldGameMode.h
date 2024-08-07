// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EnemyCharacter.h"
#include "SpawnPoint.h"
#include "Machine.h"
#include "Door.h"
#include "UnderWorldGameInstance.h"
#include "UnderWorldGameMode.generated.h"

UCLASS(minimalapi)
class AUnderWorldGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AUnderWorldGameMode();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UUnderWorldGameInstance* GameInstance;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(EditAnyWhere, Category = "UMG")
	TSubclassOf<UUserWidget> StartWidget;

	UPROPERTY(EditAnyWhere, Category = "UMG")
	TSubclassOf<UUserWidget> GameWidget;

	UPROPERTY(EditAnyWhere, Category = "UMG")
	TSubclassOf<UUserWidget> ClearWidget;

	UPROPERTY(EditAnywhere)
	USoundBase* StartSound;

	UPROPERTY(EditAnywhere)
	USoundBase* BGMSound;

	UPROPERTY(EditAnywhere)
	USoundBase* RestartSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ClearSound;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AItem>> ItemClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACharacter> EnemyCharacterClass;

	UPROPERTY()
	int MachineInstallCompleteCount = 0;

	UPROPERTY()
	ASurvivorCharacter* SurvivorCharacter;

	UPROPERTY()
	AEnemyCharacter* EnemyCharacter;

	UPROPERTY()
	TArray<ASpawnPoint*> EnemySpawnPoints;

	UPROPERTY()
	TArray<ASpawnPoint*> ItemSpawnPoints;

	UPROPERTY()
	TArray<ASpawnPoint*> OutsideItemSpawnPoints;

	UPROPERTY()
	TArray<ASpawnPoint*> InsideItemSpawnPoints;

	UPROPERTY()
	TArray<AMachine*> Machines;

	UPROPERTY()
	TArray<ADoor*> Doors;

	UPROPERTY()
	ASpawnPoint* DoorReachPoint;

	UPROPERTY()
	UAudioComponent* StartAudioComponent;

	UPROPERTY()
	UAudioComponent* BGMAudioComponent;

	UPROPERTY()
	FTimerHandle SpawnItemTimerHandle;

public:
	UFUNCTION(BlueprintCallable)
	void StartGame(int StartStage);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void ClearGame();

	UFUNCTION()
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

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