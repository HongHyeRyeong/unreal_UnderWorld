#include "EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::StartGame(int StartStage)
{
	Stage = StartStage;

	// TODO: AI ��Ʈ�ѷ� StartGame ȣ��

	if (Stage == 3)
	{
		GetCharacterMovement()->MaxWalkSpeed = 150;
	}
}

void AEnemyCharacter::RestartGame()
{
	// TODO: AI ��Ʈ�ѷ� RestartGame ȣ��
}

void AEnemyCharacter::ClearGame()
{
	// TODO: Die �ִϸ��̼� ȣ��
}

void AEnemyCharacter::AttackBySurvivor()
{

}

bool AEnemyCharacter::Teleport(FTransform Transform)
{
	return true;
}