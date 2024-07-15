// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterAnimInstance.h"
#include "EnemyCharacter.h"

void UEnemyCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Character) == false)
	{
		auto Pawn = TryGetPawnOwner();
		Character = Cast<AEnemyCharacter>(Pawn);
	}

	if (IsValid(Character))
	{
		CharacterState = Character->State;
		bIsWalking = Character->IsWalking();
	}
}

void UEnemyCharacterAnimInstance::FinishedActionAnimation()
{
	Character->SetECharacterState(EEnemyCharacterState::LAND);
}

void UEnemyCharacterAnimInstance::FinishedAttackAnimation()
{
	Character->SetECharacterState(EEnemyCharacterState::REST);
}