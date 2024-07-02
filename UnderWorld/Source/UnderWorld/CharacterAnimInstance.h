// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnderWorldCharacter.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNDERWORLD_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	AUnderWorldCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	ECharacterState CharacterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int LandState;

public:
	UFUNCTION(BlueprintCallable)
	void FinishedActionAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayLandSound();
};
