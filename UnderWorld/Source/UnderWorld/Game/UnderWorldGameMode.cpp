// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnderWorldGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "SurvivorCharacter.h"
#include "UnderWorldGameInstance.h"

AUnderWorldGameMode::AUnderWorldGameMode()
{
}

void AUnderWorldGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UUnderWorldGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	SurvivorCharacter = Cast<ASurvivorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Enemy", FoundActors);
	for (AActor* a : FoundActors)
		EnemySpawnPoints.Add(Cast<ASpawnPoint>(a));

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "Item", FoundActors);
	for (AActor* a : FoundActors)
		ItemSpawnPoints.Add(Cast<ASpawnPoint>(a));

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "OutsideItem", FoundActors);
	for (AActor* a : FoundActors)
		OutsideItemSpawnPoints.Add(Cast<ASpawnPoint>(a));

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ASpawnPoint::StaticClass(), "InsideItem", FoundActors);
	for (AActor* a : FoundActors)
		InsideItemSpawnPoints.Add(Cast<ASpawnPoint>(a));

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMachine::StaticClass(), FoundActors);
	for (AActor* a : FoundActors)
		Machines.Add(Cast<AMachine>(a));

	MachineInstallCompleteCount = 0;
}

void AUnderWorldGameMode::StartGame(int StartStage)
{
	if (GameInstance == nullptr)
		GameInstance = Cast<UUnderWorldGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GameInstance->Stage = StartStage;

	ChangeMenuWidget(StartWidget);
	StartAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), StartSound);
	StartAudioComponent->FadeIn(0.5f);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			ChangeMenuWidget(GameWidget);
			StartAudioComponent->FadeOut(0.5f, 0);
			BGMAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), BGMSound);

			SpawnItem();
			SpawnEnemy();

			if (GameInstance->IsFinalStage())
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
					const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
					UGameplayStatics::OpenLevel(GetWorld(), FName(CurrentLevelName));

					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}), 5, false);
		}), 1, false);
}

void AUnderWorldGameMode::ClearGame()
{
	SurvivorCharacter->SetECharacterState(ECharacterState::CLEAR);
	EnemyCharacter->ClearGame();
	GetWorld()->GetTimerManager().ClearTimer(SpawnItemTimerHandle);

	ChangeMenuWidget(ClearWidget);
	UGameplayStatics::PlaySound2D(GetWorld(), ClearSound);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (GameInstance->IsFinalStage())
			{
				BGMAudioComponent->FadeOut(0.5f, 0);

				GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
					{
						UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("End"))); 
					}), 0.5f, false);
			}
			else
			{
				FString NextLevelName = TEXT("Stage");
				NextLevelName += FString::FromInt(GameInstance->Stage + 1);
				UGameplayStatics::OpenLevel(GetWorld(), FName(NextLevelName));
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
	if (GameInstance->IsFinalStage())
	{
		GetWorld()->GetTimerManager().SetTimer(SpawnItemTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				int RandomItemType = FMath::RandRange(0, 5);
				int SpawnIndex = FMath::RandRange(0, ItemSpawnPoints.Num() - 1);

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

				if (RandomItemType == (int)EItemType::HAT)
				{
					int RandomItemLevel = FMath::RandRange(1, 3);
					AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::HAT], ItemSpawnPoints[SpawnIndex]->GetActorTransform(), SpawnParams);
					Item->SetLevel(RandomItemLevel);
					Item->SetLifeSpan(7);
				}
				else if (RandomItemType == (int)EItemType::BAG)
				{
					int RandomItemLevel = FMath::RandRange(1, 2);
					AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::BAG], ItemSpawnPoints[SpawnIndex]->GetActorTransform(), SpawnParams);
					Item->SetLevel(RandomItemLevel);
					Item->SetLifeSpan(7);
				}
				else
				{
					AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::GADGET], ItemSpawnPoints[SpawnIndex]->GetActorTransform(), SpawnParams);
					Item->SetLifeSpan(7);
				}

			}), 5, true);
	}
	else
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

		TArray<FTransform> SpawnPoints;
		for (ASpawnPoint* a : OutsideItemSpawnPoints)
			SpawnPoints.Add(a->GetActorTransform());

		for (int i = 0; i < GameInstance->Stage; ++i) {
			int SpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::HAT], SpawnPoints[SpawnIndex], SpawnParams);
			Item->SetLevel(i + 1);

			SpawnPoints.RemoveAt(SpawnIndex);
		}

		for (int i = 0; i < GameInstance->Stage; ++i) {
			int SpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::BAG], SpawnPoints[SpawnIndex], SpawnParams);
			Item->SetLevel(i + 1);

			SpawnPoints.RemoveAt(SpawnIndex);
		}

		for (int i = 0; i < 3; ++i) {
			int SpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::KEY], SpawnPoints[SpawnIndex], SpawnParams);

			SpawnPoints.RemoveAt(SpawnIndex);
		}

		SpawnPoints.Reset();
		for (ASpawnPoint* a : InsideItemSpawnPoints)
			SpawnPoints.Add(a->GetActorTransform());

		for (int i = 0; i < 10; ++i) {
			int SpawnIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
			AItem* Item = GetWorld()->SpawnActor<AItem>(ItemClass[(int)EItemType::GADGET], SpawnPoints[SpawnIndex], SpawnParams);

			SpawnPoints.RemoveAt(SpawnIndex);
		}
	}
}

void AUnderWorldGameMode::SpawnEnemy()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			int SpawnIndex;
			FTransform SpawnTransform;

			if (GameInstance->IsFinalStage())
			{
				SpawnIndex = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);
				SpawnTransform = EnemySpawnPoints[SpawnIndex]->GetActorTransform();
			}
			else
			{
				SpawnIndex = FMath::RandRange(0, Machines.Num() - 1);
				SpawnTransform = Machines[SpawnIndex]->GetRandomTransform(700);
			}

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
			EnemyCharacter = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyCharacterClass, SpawnTransform, SpawnParams);
			EnemyCharacter->StartGame();

			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}), 5, false);
}

void AUnderWorldGameMode::TeleportEnemy()
{
	if (IsValid(EnemyCharacter) == false)
		return;

	FTransform TeleportTransform;

	if (GameInstance->IsFinalStage())
	{
		SetDoorReachPoint();

		int RandomIndex = FMath::RandRange(0, EnemySpawnPoints.Num() - 1);
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
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				TeleportEnemy();
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
			}), 1, false);
	}
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
		if (GameInstance->IsFinalStage() && MachineInstallCompleteCount >= (Machines.Num() / 2))
		{
			EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 250;
		}
	}
}

void AUnderWorldGameMode::SetDoorReachPoint()
{
	int RandomIndex = FMath::RandRange(0, Doors.Num() - 1);
	DoorReachPoint->SetActorLocation(Doors[RandomIndex]->GetActorLocation());
	Doors[RandomIndex]->bIsReachDoor = true;
}