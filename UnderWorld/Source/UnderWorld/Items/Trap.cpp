// Fill out your copyright notice in the Description page of Project Settings.

#include "Trap.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "SurvivorCharacter.h"
#include "EnemyCharacter.h"

ATrap::ATrap()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(10, 10, 10));
    CollisionBox->SetCollisionProfileName(TEXT("OverlapOnlySurvivor"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ATrap::OnBeginOverlap);
}

void ATrap::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASurvivorCharacter* SurvivorCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (SurvivorCharacter)
    {
        SurvivorCharacter->AttackByTrap();
        SurvivorCharacter->OnChangeState.AddDynamic(this, &ATrap::DestroyTrap);

        // 트랩 발동 시 적 캐릭터 순간 이동
        AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyCharacter::StaticClass()));
        if (GetDistanceTo(EnemyCharacter) > 100)
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
                {
                    FVector Location = GetActorLocation() + FVector(50, 0, 0);
                    EnemyCharacter = Cast<AEnemyCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyCharacter::StaticClass()));
                    EnemyCharacter->Teleport(UKismetMathLibrary::MakeTransform(Location, UKismetMathLibrary::FindLookAtRotation(Location, GetActorLocation())));
                    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
                }), 4, false);

        }
    }
}

void ATrap::DestroyTrap(ECharacterState State)
{
    Destroy();
}