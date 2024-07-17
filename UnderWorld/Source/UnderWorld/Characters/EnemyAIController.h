// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"

UCLASS()
class UNDERWORLD_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<UBehaviorTree*> BehaviorTree;

	UPROPERTY()
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Sight* AIPerceptionSignt;

	UPROPERTY(EditAnywhere)
	USoundBase* ChaseSound;

	UPROPERTY()
	UAudioComponent* ChaseAudioComponent;

	UPROPERTY()
	int Stage = 0;

	UPROPERTY()
	FTimerHandle LineOfSightTimerHandle;

	UPROPERTY()
	FTimerHandle TeleportTimerHandle;

public:
	UFUNCTION()
	void StartGame(int StartStage);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void CheckTeleport();

	UFUNCTION()
	void FinishedTeleport();

	UFUNCTION()
	void ComeInEnemy();

	UFUNCTION()
	void SetBlackboardActionValue(bool bIsAction);

	UFUNCTION()
	void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
