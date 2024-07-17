// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivorCharacterAnimInstance.h"
#include "SurvivorCharacter.h"

void USurvivorCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Character) == false)
	{
		auto Pawn = TryGetPawnOwner();
		Character = Cast<ASurvivorCharacter>(Pawn);
	}

	if (IsValid(Character))
	{
		CharacterState = Character->State;

		LandState = 0;
		if (Character->IsWalking())
		{
			LandState = 1;

			if (Character->IsRunning())
			{
				LandState = 2;
			}
		}
	}
}

void USurvivorCharacterAnimInstance::AttackToEnemy()
{
	Character->OnCounterAttackToEnemy.Broadcast();
}

void USurvivorCharacterAnimInstance::FinishedActionAnimation()
{
	Character->SetECharacterState(ECharacterState::LAND);
}