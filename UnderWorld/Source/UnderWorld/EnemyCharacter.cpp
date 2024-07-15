#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyAIController.h"
#include "UnderWorldCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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

	SurvivorCharacter = Cast<AUnderWorldCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AEnemyCharacter::SetECharacterState(EEnemyCharacterState NewState)
{
	State = NewState;

	AttackCollision->SetGenerateOverlapEvents(NewState == EEnemyCharacterState::ATTACK);

	if (State == EEnemyCharacterState::LAND)
	{
		// ai 컨트롤러 Action false
	}
	else
	{
		// ai 컨트롤러 Action true
	}
}

void AEnemyCharacter::StartGame(int StartStage)
{
	Stage = StartStage;

	// TODO: AI 컨트롤러 StartGame 호출

	if (Stage == 3)
	{
		GetCharacterMovement()->MaxWalkSpeed = 150;
	}
}

void AEnemyCharacter::RestartGame()
{
	// TODO: AI 컨트롤러 RestartGame 호출
}

void AEnemyCharacter::ClearGame()
{
	// TODO: Die 애니메이션 호출
}

void AEnemyCharacter::OnBeginOverlapAttackCollision(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AttackCollision->SetGenerateOverlapEvents(false);

	if (SurvivorCharacter->state == ECharacterState::COUNTER_ATTACK)
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
	return true;
}

bool AEnemyCharacter::IsWalking() const
{
	return GetCharacterMovement()->Velocity.Length() > 0;
}