// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "UnderWorldGameMode.h"

void AEnemyAIController::StartGame(int StartStage)
{
	Stage = StartStage;

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (Stage == 3)
			{
				RunBehaviorTree(BehaviorTree[1]);
			}
			else
			{
				RunBehaviorTree(BehaviorTree[0]);

				AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
				AIPerceptionSignt = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

				if (AIPerceptionComponent && AIPerceptionSignt)
				{
					AIPerceptionComponent->ConfigureSense(*AIPerceptionSignt);
					AIPerceptionComponent->SetDominantSense(AIPerceptionSignt->GetSenseImplementation());

					AIPerceptionSignt->SightRadius = 1500.f;
					AIPerceptionSignt->LoseSightRadius = 2000.f;
					AIPerceptionSignt->PeripheralVisionAngleDegrees = 60.0f;

					AIPerceptionSignt->DetectionByAffiliation.bDetectEnemies = true;
					AIPerceptionSignt->DetectionByAffiliation.bDetectNeutrals = true;
					AIPerceptionSignt->DetectionByAffiliation.bDetectFriendlies = false;
				}

				AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);

				CheckTeleport();
			}

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}), 1, false);

	ChaseAudioComponent = UGameplayStatics::SpawnSoundAttached(ChaseSound, GetPawn()->GetRootComponent());
	ChaseAudioComponent->Stop();
}

void AEnemyAIController::RestartGame()
{
	if (ChaseAudioComponent->IsPlaying())
		ChaseAudioComponent->FadeOut(0.5f, 0);
}

void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// TODO: 도달 가능한 위치인지? 체크?

	if (Actor->ActorHasTag("Player") && Stimulus.SensingSucceeded)
	{
		GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", true);
		GetBlackboardComponent()->SetValueAsObject("EnemyActor", Actor);

		GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

		if (ChaseAudioComponent->IsPlaying() == false)
			ChaseAudioComponent->FadeIn(0.5f);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(LineOfSightTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", false);
				GetBlackboardComponent()->SetValueAsObject("EnemyActor", NULL);

				GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
			}), 2, false);

		CheckTeleport();

		if (ChaseAudioComponent->IsPlaying())
			ChaseAudioComponent->FadeOut(0.5f, 0);
	}
}

void AEnemyAIController::CheckTeleport()
{
	GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			Cast<AUnderWorldGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->TeleportEnemy();
			GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);
		}), 15, false);
}

void AEnemyAIController::FinishedTeleport()
{
	if (Stage != 3)
		CheckTeleport();

	GetBlackboardComponent()->SetValueAsBool("Teleport", true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetBlackboardComponent()->SetValueAsBool("Teleport", false);
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}), 2, false);
}

void AEnemyAIController::ComeInEnemy()
{
	GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", true);
	GetBlackboardComponent()->SetValueAsObject("EnemyActor", Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)));

	if (ChaseAudioComponent->IsPlaying() == false)
		ChaseAudioComponent->FadeIn(0.5f);
}

void AEnemyAIController::SetBlackboardActionValue(bool bIsAction)
{
	GetBlackboardComponent()->SetValueAsBool("Action", bIsAction);
}