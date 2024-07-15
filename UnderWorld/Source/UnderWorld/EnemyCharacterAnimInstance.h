// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyCharacter.h"
#include "EnemyCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNDERWORLD_API UEnemyCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	AEnemyCharacter* Character;

	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EEnemyCharacterState CharacterState;

	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool bIsWalking = false;

public:
	UFUNCTION(BlueprintCallable)
	void FinishedActionAnimation();

	UFUNCTION(BlueprintCallable)
	void FinishedAttackAnimation();
};
