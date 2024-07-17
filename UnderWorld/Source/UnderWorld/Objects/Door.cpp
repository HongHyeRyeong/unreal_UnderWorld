// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "UnderWorldGameMode.h"
#include "EnemyAIController.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;

	ButtonCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollision"));
	ButtonCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ButtonCollision->SetCollisionProfileName(TEXT("OverlapOnlySurvivor"));
	ButtonCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBeginOverlapButton);
	ButtonCollision->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnEndOverlapButton);

	ComeInEnemyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ComeInEnemyCollision"));
	ComeInEnemyCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ComeInEnemyCollision->SetCollisionProfileName(TEXT("OverlapOnlyEnemy"));
	ComeInEnemyCollision->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBeginOverlapComeInEnemy);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyCharacter::StaticClass());
	EnemyCharacter = Cast<AEnemyCharacter>(FoundActor);
	bIsComeInEnemy = false;

	DoorOpen();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsComeInEnemy == false)
	{
		float Distance = EnemyCharacter->GetDistanceTo(this);
		int NewDoorLightState = 0;

		if (Distance <= 500)
			DoorLightState = 3;
		else if (Distance <= 1000)
			DoorLightState = 2;
		else if (Distance <= 1500)
			DoorLightState = 1;

		if (DoorLightState != NewDoorLightState)
		{
			DoorLightState = NewDoorLightState;
			SetDoorLightState();
		}
	}
}

void ADoor::OnBeginOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ButtonDown();

	if (bIsComeInEnemy)
		return;

	GetWorld()->GetTimerManager().SetTimer(ButtonTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			SetDoorOpen(false);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					SetDoorOpen(true);

					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}), 3, false);

			GetWorld()->GetTimerManager().ClearTimer(ButtonTimerHandle);
		}), 0.5f, false);
}

void ADoor::OnEndOverlapButton(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ButtonUp();

	if (bIsComeInEnemy)
		return;

	GetWorld()->GetTimerManager().ClearTimer(ButtonTimerHandle);
}

void ADoor::OnBeginOverlapComeInEnemy(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOpenDoor)
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADoor::StaticClass(),FoundActors);
		for (AActor* a : FoundActors)
		{
			ADoor* Door = Cast<ADoor>(a);
			Door->ComeInEnemy();
		}

		Cast<AEnemyAIController>(EnemyCharacter->GetController())->ComeInEnemy();
	}
}

void ADoor::SetDoorOpen(bool bIsOpen)
{
	bIsOpenDoor = bIsOpen;

	if(bIsOpenDoor)
		DoorOpen();
	else 
	{
		DoorClose();

		if (bIsReachDoor)
		{
			bIsReachDoor = false;

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
				{
					AUnderWorldGameMode* GameMode = Cast<AUnderWorldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
					GameMode->TeleportEnemy();

					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
				}), 2, false);
		}
	}
}

void ADoor::ComeInEnemy()
{
	bIsComeInEnemy = true;

	DoorLightState = 3;
	SetDoorLightState();

	DoorClose();
	GetWorld()->GetTimerManager().ClearTimer(ButtonTimerHandle);
}