// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyAIController.h"
#include "SurvivorCharacter.h"
#include "Trap.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(45.f, 20.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PawnEnemy"));
	GetCapsuleComponent()->bReceivesDecals = false;
	GetMesh()->SetReceivesDecals(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	AttackCollision->SetupAttachment(GetMesh(), FName("Bip001-R-Hand"));
	AttackCollision->SetSphereRadius(12);
	AttackCollision->SetCollisionProfileName(TEXT("OverlapOnlySurvivor"));
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnBeginOverlapAttackCollision);
	AttackCollision->SetGenerateOverlapEvents(false);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIController = Cast<AEnemyAIController>(GetController());
	SurvivorCharacter = Cast<ASurvivorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AEnemyCharacter::StartGame(int StartStage)
{
	Stage = StartStage;

	EnemyAIController->StartGame(StartStage);

	if (Stage == 3)
		SetMaxWalkSpeed(150);
}

void AEnemyCharacter::RestartGame()
{
	EnemyAIController->RestartGame();
}

void AEnemyCharacter::OnBeginOverlapAttackCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AttackCollision->SetGenerateOverlapEvents(false);

	if (SurvivorCharacter->State == ECharacterState::COUNTER_ATTACK)
	{
		SurvivorCharacter->OnCounterAttackToEnemy.AddDynamic(this, &AEnemyCharacter::AttackBySurvivor);
	}
	else
	{
		bool Front = UKismetMathLibrary::Dot_VectorVector(SurvivorCharacter->GetActorForwardVector(), GetActorForwardVector()) >= 0;
		SurvivorCharacter->AttackByEnemy(Front);
	}
}

void AEnemyCharacter::AttackBySurvivor()
{
	SetECharacterState(EEnemyCharacterState::DOWN);
	SurvivorCharacter->OnCounterAttackToEnemy.Clear();
}

bool AEnemyCharacter::Teleport(FTransform Transform)
{
	if (State != EEnemyCharacterState::LAND)
		return false;

	bool bIsSuccess = SetActorTransform(Transform, false, nullptr, ETeleportType::TeleportPhysics);
	if (bIsSuccess == false)
		return false;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TeleportSound, GetActorLocation());

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;
	GetWorld()->SpawnActor<AActor>(TeleportEffectClass, GetActorTransform(), SpawnParams);

	EnemyAIController->FinishedTeleport();

	return true;
}

void AEnemyCharacter::InstallTrap()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	FTransform SpawnTransform = GetActorTransform();
	SpawnTransform.SetLocation(SpawnTransform.GetLocation() + FVector(38, 0, -48));

	GetWorld()->SpawnActor<ATrap>(TrapClass, SpawnTransform, SpawnParams);
	SetECharacterState(EEnemyCharacterState::TRAP);
}

void AEnemyCharacter::SetECharacterState(EEnemyCharacterState NewState)
{
	State = NewState;

	AttackCollision->SetGenerateOverlapEvents(NewState == EEnemyCharacterState::ATTACK);
	EnemyAIController->SetBlackboardActionValue(State != EEnemyCharacterState::LAND);
}

void AEnemyCharacter::SetMaxWalkSpeed(float MaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

bool AEnemyCharacter::IsWalking() const
{
	return GetCharacterMovement()->Velocity.Length() > 0;
}