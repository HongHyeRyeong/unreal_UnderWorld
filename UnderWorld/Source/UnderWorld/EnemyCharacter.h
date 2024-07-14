// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class UNDERWORLD_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite)
	int Stage = 0;

	UFUNCTION()
	void StartGame(int StartStage);

	UFUNCTION()
	void RestartGame();

	UFUNCTION()
	void ClearGame();

	void AttackBySurvivor();
	bool Teleport(FTransform Transform);
};
