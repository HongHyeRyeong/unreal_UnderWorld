#include "ItemBase.h"
#include "UnderWorldCharacter.h"
#include "InventoryComponent.h"

AItemBase::AItemBase()
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(15, 15, 15));
    CollisionBox->SetCollisionProfileName(TEXT("OverlapOnlySurvivor"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnEndOverlap);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
    StaticMesh->bReceivesDecals = false;
}

void AItemBase::SetLevel(int InitLevel)
{
    Level = InitLevel;
    StaticMesh->SetMaterial(0, Materials[InitLevel]);
}

void AItemBase::SetOutline(bool Active)
{
    StaticMesh->SetOverlayMaterial(Active ? Materials[0] : NULL);
}

void AItemBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AUnderWorldCharacter* OtherCharacter = Cast<AUnderWorldCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->InventoryComponent->BeginOverlap(this);
    }
}

void AItemBase::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    AUnderWorldCharacter* OtherCharacter = Cast<AUnderWorldCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->InventoryComponent->EndOverlap(this);
    }
}