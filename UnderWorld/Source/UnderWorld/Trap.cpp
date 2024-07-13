#include "Trap.h"
#include "UnderWorldCharacter.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
    AUnderWorldCharacter* OtherCharacter = Cast<AUnderWorldCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->AttackByTrap();
        OtherCharacter->OnChangeState.AddDynamic(this, &ATrap::DestroyTrap);

        // 트랩 발동 시 적 캐릭터 순간 이동
        AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyCharacter::StaticClass());
        float Distance = GetDistanceTo(FoundActor);

        if (Distance > 100)
        {
            FTimerHandle TimerHandle;
            float DelayTime = 4;

            GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]()
                {
                    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(FoundActor);
                    FVector Location = GetActorLocation() + FVector(50, 0, 0);
                    Enemy->Teleport(UKismetMathLibrary::MakeTransform(Location, UKismetMathLibrary::FindLookAtRotation(Location, GetActorLocation())));
                    GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
                }), DelayTime, false);

        }
    }
}

void ATrap::DestroyTrap(ECharacterState State)
{
    Destroy();
}