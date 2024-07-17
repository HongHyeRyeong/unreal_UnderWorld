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
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	ASurvivorCharacter* Character;

	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int LandState;

public:
	UFUNCTION(BlueprintCallable)
	void AttackToEnemy();

	UFUNCTION(BlueprintCallable)
	void FinishedActionAnimation();
};
