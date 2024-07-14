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

void AEnemyCharacter::AttackBySurvivor()
{

}

bool AEnemyCharacter::Teleport(FTransform Transform)
{
	return true;
}