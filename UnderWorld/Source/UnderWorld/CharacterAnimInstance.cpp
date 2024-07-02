// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "UnderWorldCharacter.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Character) == false)
	{
		auto Pawn = TryGetPawnOwner();
		Character = Cast<AUnderWorldCharacter>(Pawn);
	}

	if (IsValid(Character))
	{
		CharacterState = Character->state;

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

	PlayLandSound();
}

void UCharacterAnimInstance::FinishedActionAnimation()
{
	Character->SetECharacterState(ECharacterState::LAND);
}