// Fill out your copyright notice in the Description page of Project Settings.

#include "Prison.h"
#include "SurvivorCharacter.h"
#include "Components/CapsuleComponent.h"

APrison::APrison()
{
	PrimaryActorTick.bCanEverTick = false;

    CollisionBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    RootComponent = CollisionBox;
    CollisionBox->SetCapsuleHalfHeight(190);
    CollisionBox->SetCapsuleRadius(110);
    CollisionBox->SetCollisionProfileName(TEXT("OverlapOnlySurvivor"));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APrison::OnBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APrison::OnEndOverlap);
}

void APrison::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASurvivorCharacter* OtherCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (OtherCharacter)
    {
        OtherCharacter->OnInputPrison.AddDynamic(this, &APrison::DoorOpen);
    }
}

void APrison::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ASurvivorCharacter* OtherCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (OtherCharacter)
    {
        DoorClose();
        OtherCharacter->OnInputPrison.Clear();
    }
}