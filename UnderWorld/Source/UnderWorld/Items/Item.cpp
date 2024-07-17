// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "SurvivorCharacter.h"
#include "InventoryComponent.h"

AItem::AItem()
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(15, 15, 15));
    CollisionBox->SetCollisionProfileName(TEXT("OverlapOnlySurvivor"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
    StaticMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
    StaticMesh->bReceivesDecals = false;
}

void AItem::SetLevel(int InitLevel)
{
    Level = InitLevel;
    StaticMesh->SetMaterial(0, Materials[InitLevel]);
}

void AItem::SetOutline(bool Active)
{
    StaticMesh->SetOverlayMaterial(Active ? Materials[0] : NULL);
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASurvivorCharacter* OtherCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->InventoryComponent->BeginOverlap(this);
    }
}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ASurvivorCharacter* OtherCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->InventoryComponent->EndOverlap(this);
    }
}