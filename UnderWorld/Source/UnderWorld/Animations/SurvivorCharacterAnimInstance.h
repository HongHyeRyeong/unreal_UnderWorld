// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SurvivorCharacter.h"
#include "SurvivorCharacterAnimInstance.generated.h"

UCLASS()
class UNDERWORLD_API USurvivorCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	ASurvivorCharacter* Character;

	UPROPERTY(BlueprintReadWrite)
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly)
	float CharacterSpeed;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	void AttackToEnemy();

	UFUNCTION(BlueprintCallable)
	void FinishedActionAnimation();
};
