// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnderWorldCharacter.h"
#include "Components/AudioComponent.h"
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

	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadWrite, Category = Pawn, Meta = (AllowPrivateAccess = true))
	int LandState;

public:
	UPROPERTY(EditAnywhere)
	USoundBase* WalkSound;

	UPROPERTY()
	UAudioComponent* WalkAudioComponent;

	UFUNCTION(BlueprintCallable)
	void FinishedActionAnimation();
};
