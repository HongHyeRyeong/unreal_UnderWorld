// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnderWorldGameMode.h"
#include "UnderWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AUnderWorldGameMode::AUnderWorldGameMode()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMachine::StaticClass(), FoundActors);
	for (AActor* a : FoundActors)
		Machines.Add(Cast<AMachine>(a));

	MachineInstallCompleteCount = 0;
}

void AUnderWorldGameMode::SpawnEnemyByMachine()
{
	int SpawnIndex = FMath::RandRange(0, Machines.Num());
	FTransform SpawnTransform = Machines[SpawnIndex]->GetRandomTransform(700);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterClass, SpawnTransform, SpawnParams);
}

void AUnderWorldGameMode::TeleportEnemy()
{
	if (Stage == 3)
	{

	}
	else
		TeleportEnemyByMachine();
}

void AUnderWorldGameMode::TeleportEnemyByMachine()
{
	TArray<AActor*> Actors;
	float Distance;
	for (AMachine* a : Machines)
		Actors.Add(Cast<AActor>(a));

	AActor* NearestActor = UGameplayStatics::FindNearestActor(EnemyCharacter->GetActorLocation(), Actors, Distance);
	AMachine* NearestMachine = Cast<AMachine>(NearestActor);

	AMachine* FoundMachine = NearestMachine;
	int InstallCount = 0;

	for (AMachine* a : Machines)
	{
		if (a == NearestMachine) // Nearest
			continue;
		if (a->InstallCount >= a->InstallCompleteCount) // Install Complete
			continue;
		if (InstallCount > a->InstallCount) // Most Install Count
			continue;

		FoundMachine = a;
		InstallCount = a->InstallCount;
	}

	float Radius = FoundMachine->InstallCount > (FoundMachine->InstallCompleteCount - 3) ? 700 : 1200;
	bool bIsSuccess = EnemyCharacter->Teleport(FoundMachine->GetRandomTransform(Radius));

	if (bIsSuccess == false)
		TeleportEnemyByMachine();
}

void AUnderWorldGameMode::CompleteMachineInstall()
{
	MachineInstallCompleteCount++;

	if (MachineInstallCompleteCount >= Machines.Num()) // Clear
	{
		// Clear Game È£Ãâ
	}
	else
	{
		if (Stage == 3 && MachineInstallCompleteCount >= (Machines.Num() / 2))
		{
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 250;
		}
	}
}