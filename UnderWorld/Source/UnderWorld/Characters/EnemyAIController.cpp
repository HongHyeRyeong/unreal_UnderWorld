// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/LogMacros.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "UnderWorldGameMode.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	AIPerceptionSignt = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");

	AIPerceptionComponent->ConfigureSense(*AIPerceptionSignt);
	AIPerceptionComponent->SetDominantSense(AIPerceptionSignt->GetSenseImplementation());

	AIPerceptionSignt->SightRadius = 1500.f;
	AIPerceptionSignt->LoseSightRadius = 2000.f;
	AIPerceptionSignt->PeripheralVisionAngleDegrees = 60.0f;
	AIPerceptionSignt->DetectionByAffiliation.bDetectEnemies = true;
	AIPerceptionSignt->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionSignt->DetectionByAffiliation.bDetectFriendlies = false;

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);
	AIPerceptionComponent->SetSenseEnabled(AIPerceptionSignt->GetClass(), false);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UUnderWorldGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AEnemyAIController::StartGame()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (GameInstance->IsFinalStage())
			{
				RunBehaviorTree(BehaviorTree[1]);

				TArray<AActor*> FoundActors;
				UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Door", FoundActors);
				GetBlackboardComponent()->SetValueAsObject("EnemyActor", FoundActors[0]);
			}
			else
			{
				RunBehaviorTree(BehaviorTree[0]);
				AIPerceptionComponent->SetSenseEnabled(AIPerceptionSignt->GetClass(), true);

				CheckTeleport();
			}

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}), 1, false);
}

void AEnemyAIController::RestartGame()
{
	if (ChaseAudioComponent != nullptr)
		ChaseAudioComponent->FadeOut(0.5f, 0);
}

void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// TODO: 도달 가능한 위치인지? 체크?

	if (Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed())
	{
		GetBlackboardComponent()->SetValueAsBool("HasLineOfSight", true);
		GetBlackboardComponent()->SetValueAsObject("EnemyActor", Actor);

		GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

		ChaseAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), ChaseSound);
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

		if (ChaseAudioComponent != nullptr)
			ChaseAudioComponent->FadeOut(0.5f, 0);
	}
}

void AEnemyAIController::CheckTeleport()
{
	GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			GetWorld()->GetTimerManager().ClearTimer(TeleportTimerHandle);
			Cast<AUnderWorldGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->TeleportEnemy();
		}), 15, false);
}

void AEnemyAIController::FinishedTeleport()
{
	if (GameInstance->IsFinalStage() == false)
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

	if (ChaseAudioComponent->VolumeMultiplier == 0)
		ChaseAudioComponent->FadeIn(0.5f);
}

void AEnemyAIController::SetBlackboardActionValue(bool bIsAction)
{
	GetBlackboardComponent()->SetValueAsBool("Action", bIsAction);
}