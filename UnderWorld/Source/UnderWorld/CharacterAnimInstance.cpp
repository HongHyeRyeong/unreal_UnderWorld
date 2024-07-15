// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "UnderWorldCharacter.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

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

		WalkAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), WalkSound);
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

		if (CharacterState == ECharacterState::LAND && LandState != 0)
		{
			if (WalkAudioComponent->IsPlaying() == false)
				WalkAudioComponent->Play();

			float PitchMultiplier = LandState == 1 ? 0.9f : 1.2f;
			if (WalkAudioComponent->PitchMultiplier != PitchMultiplier)
				WalkAudioComponent->SetPitchMultiplier(PitchMultiplier);
		}
		else
		{
			if (WalkAudioComponent->IsPlaying())
				WalkAudioComponent->Stop();
		}
	}
}

void UCharacterAnimInstance::FinishedActionAnimation()
{
	Character->SetECharacterState(ECharacterState::LAND);
}