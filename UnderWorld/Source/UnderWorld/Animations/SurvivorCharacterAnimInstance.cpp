// Fill out your copyright notice in the Description page of Project Settings.

#include "SurvivorCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		if (CharacterState != Character->State && Character->State == ECharacterState::MACHINE_INSTALL)
		{
			Montage_Play(MachineMontage);
		}

		if (CharacterState != Character->State && Character->State != ECharacterState::MACHINE_INSTALL)
		{
			Montage_Stop(0.2f, MachineMontage);
		}

		CharacterState = Character->State;
		CharacterSpeed = Character->GetCharacterMovement()->Velocity.Length();
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