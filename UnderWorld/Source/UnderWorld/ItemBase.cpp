#include "ItemBase.h"
#include "UnderWorldCharacter.h"
#include "InventoryComponent.h"

AItemBase::AItemBase()
{
    if (!CollisionBox)
    {
        CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
        RootComponent = CollisionBox;

        CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnBeginOverlap);
        CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnEndOverlap);
    }
}

void AItemBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUnderWorldCharacter* character = Cast<AUnderWorldCharacter>(OtherActor);
	if (character)
	{
        character->InventoryComponent->BeginOverlap(this);
	}
}

void AItemBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AUnderWorldCharacter* character = Cast<AUnderWorldCharacter>(OtherActor);
    if (character)
    {
        character->InventoryComponent->EndOverlap(this);
    }
}