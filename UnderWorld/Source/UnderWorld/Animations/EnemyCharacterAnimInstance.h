// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyCharacter.h"
#include "EnemyCharacterAnimInstance.generated.h"

UCLASS()
class UNDERWORLD_API UEnemyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	AEnemyCharacter* Character;

	UPROPERTY(BlueprintReadWrite)
	EEnemyCharacterState CharacterState;

	UPROPERTY(BlueprintReadWrite)
	bool bIsWalking = false;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void FinishedActionAnimation();

	UFUNCTION(BlueprintCallable)
	void FinishedAttackAnimation();
};
