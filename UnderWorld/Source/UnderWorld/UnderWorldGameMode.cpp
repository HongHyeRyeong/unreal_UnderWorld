// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnderWorldGameMode.h"
#include "UnderWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

AUnderWorldGameMode::AUnderWorldGameMode()
{
}

void AUnderWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	SurvivorCharacter = Cast<AUnderWorldCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Enemy", FoundActors);
	for (AActor* a : FoundActors)
		EnemySpawnPoints.Add(Cast<ASpawnPoint>(a));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMachine::StaticClass(), FoundActors);
	for (AActor* a : FoundActors)
		Machines.Add(Cast<AMachine>(a));

	MachineInstallCompleteCount = 0;
}

void AUnderWorldGameMode::StartGame(int StartStage)
{
	Stage = StartStage;

	ChangeMenuWidget(StartWidget);
	UAudioComponent* StartAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), StartSound);
	StartAudioComponent->FadeIn(0.5f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			ChangeMenuWidget(GameWidget);
			StartAudioComponent->FadeOut(0.5f, 0);
			BGMAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BGMSound);

			SpawnItem();
			SpawnEnemy();

			if (Stage == 3)
			{
				TArray<AActor*> FoundActors;

				UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(), FoundActors);
				for (AActor* a : FoundActors)
					Doors.Add(Cast<ADoor>(a));

				UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Door", FoundActors);
				for (AActor* a : FoundActors)
					DoorReachPoint = Cast<ASpawnPoint>(a);

				SetDoorReachPoint();
			}

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}), 4, false);
}

void AUnderWorldGameMode::RestartGame()
{
	EnemyCharacter->RestartGame();
	UGameplayStatics::PlaySound2D(GetWorld(), RestartSound);
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.3f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.f);

			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Stage" + Stage)));

					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}), 5, false);
		}), 1, false);
}

void AUnderWorldGameMode::ClearGame()
{
	SurvivorCharacter->SetECharacterState(ECharacterState::CLEAR);
	EnemyCharacter->ClearGame();

	ChangeMenuWidget(ClearWidget);
	UGameplayStatics::PlaySound2D(GetWorld(), ClearSound);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (Stage == 3)
			{
				BGMAudioComponent->FadeOut(0.5f, 0);

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("End")));
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					}), 0.5f, false);
			}
			else
			{
				UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Stage" + (Stage + 1))));
			}
		}), 10, false);
}

void AUnderWorldGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (CurrentWidget != nullptr) {
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr) {
		CurrentWidget = CreateWidget(GetWorld(), NewWidgetClass);
		if (CurrentWidget != nullptr)
			CurrentWidget->AddToViewport();
	}
}

void AUnderWorldGameMode::SpawnItem()
{
	TArray<AActor*> FoundActors;
	int HatMaxLevel = Stage;
	int BagMaxLevel = FMath::Min(Stage, 2);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	if (Stage == 3)
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Item", FoundActors);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				AItemBase* Item;
				int RandomItemType = FMath::RandRange(0, 5);
				int SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num());

				if (RandomItemType == (int)EItemType::HAT)
				{
					int RandomItemLevel = FMath::RandRange(1, HatMaxLevel);
					Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::HAT], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);
					Item->SetLevel(RandomItemLevel);
				}
				else if (RandomItemType == (int)EItemType::BAG)
				{
					int RandomItemLevel = FMath::RandRange(1, BagMaxLevel);
					Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::BAG], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);
					Item->SetLevel(RandomItemLevel);
				}
				else
				{
					Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::GADGET], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);
				}

				Item->SetLifeSpan(7);
			}), 5, true);
	}
	else
	{
		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Item", FoundActors);

		for (int i = 0; i < HatMaxLevel; ++i) {
			int SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num());
			AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::HAT], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);
			Item->SetLevel(i + 1);

			FoundActors.Remove(FoundActors[SpawnIndex]);
		}

		for (int i = 0; i < BagMaxLevel; ++i) {
			int SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num());
			AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::BAG], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);
			Item->SetLevel(i + 1);

			FoundActors.Remove(FoundActors[SpawnIndex]);
		}

		for (int i = 0; i < 3; ++i) {
			int SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num());
			AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::KEY], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);

			FoundActors.Remove(FoundActors[SpawnIndex]);
		}

		UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Gadget", FoundActors);

		for (int i = 0; i < 10; ++i) {
			int SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num());
			AItemBase* Item = GetWorld()->SpawnActor<AItemBase>(ItemClass[(int)EItemType::GADGET], FoundActors[SpawnIndex]->GetActorTransform(), SpawnParams);

			FoundActors.Remove(FoundActors[SpawnIndex]);
		}
	}
}

void AUnderWorldGameMode::SpawnEnemy()
{
	FTimerHandle TimerHandle;
	float DelayTime = 5;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			int SpawnIndex;
			FTransform SpawnTransform;

			if (Stage == 3)
			{
				SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num());
				SpawnTransform = EnemySpawnPoints[SpawnIndex]->GetActorTransform();
			}
			else
			{
				SpawnIndex = FMath::RandRange(0, Machines.Num());
				SpawnTransform = Machines[SpawnIndex]->GetRandomTransform(700);
			}

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterClass, SpawnTransform, SpawnParams);
			EnemyCharacter->StartGame(Stage);

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}), DelayTime, false);
}

void AUnderWorldGameMode::TeleportEnemy()
{
	FTransform TeleportTransform;

	if (Stage == 3)
	{
		SetDoorReachPoint();

		int RandomIndex = FMath::RandRange(0, EnemySpawnPoints.Num());
		TeleportTransform = EnemySpawnPoints[RandomIndex]->GetActorTransform();
	}
	else
	{
		TArray<AActor*> Actors;
		for (AMachine* a : Machines)
			Actors.Add(Cast<AActor>(a));

		float Distance;
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
		TeleportTransform = FoundMachine->GetRandomTransform(Radius);
	}

	if (EnemyCharacter->Teleport(TeleportTransform) == false)
		TeleportEnemy();
}

void AUnderWorldGameMode::CompleteMachineInstall()
{
	MachineInstallCompleteCount++;

	if (MachineInstallCompleteCount >= Machines.Num())
	{
		ClearGame();
	}
	else
	{
		if (Stage == 3 && MachineInstallCompleteCount >= (Machines.Num() / 2))
		{
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 250;
		}
	}
}

void AUnderWorldGameMode::SetDoorReachPoint()
{
	int RandomIndex = FMath::RandRange(0, Doors.Num());
	DoorReachPoint->SetActorLocation(Doors[RandomIndex]->GetActorLocation());
	Doors[RandomIndex]->bIsReachDoor = true;
}