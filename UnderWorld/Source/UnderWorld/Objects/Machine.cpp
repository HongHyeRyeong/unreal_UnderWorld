// Fill out your copyright notice in the Description page of Project Settings.

#include "Machine.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SpotLightComponent.h"
#include "Sound/SoundBase.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnderWorldGameMode.h"

AMachine::AMachine()
{
	PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent = Root;

    MachineWhellRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MachineWhellRoot"));
    MachineWhellRoot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    EnemySpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EnemySpawnPoint"));
    EnemySpawnPoint->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

    InstallParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("InstallParticleSystem"));
    InstallParticleSystem->SetupAttachment(RootComponent);

    CompleteLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("CompleteLight"));
    CompleteLight->SetupAttachment(RootComponent);
}

void AMachine::BeginPlay()
{
	Super::BeginPlay();

    InstallCount = 0;
    InstallCompleteCount = 3;

    TArray<UBoxComponent*> InstallCollisions;
    this->GetComponents<UBoxComponent>(InstallCollisions);
    for (UBoxComponent* Collision : InstallCollisions)
    {
        Collision->OnComponentBeginOverlap.AddDynamic(this, &AMachine::OnBeginOverlap);
        Collision->OnComponentEndOverlap.AddDynamic(this, &AMachine::OnEndOverlap);
    }

    InstallParticleSystem->SetVisibility(true);
    CompleteLight->SetVisibility(false);
}

void AMachine::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    bool IsCanStall = false;

    if (bIsOverlap && bIsInput)
        if (IsValid(Character) && Character->IsHaveGadget())
            if (InstallCount < InstallCompleteCount)
                IsCanStall = true;

    if (IsCanStall)
    {
        if (bIsInstall == false)
        {
            bIsInstall = true;

            Character->SetECharacterState(ECharacterState::MACHINE_INSTALL);

            if (IsValid(InstallAudioComponent) == false)
                InstallAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), InstallSound);
            InstallAudioComponent->FadeIn(0.5f);
        }

        InstallGauge += Character->InstallSpeed * DeltaTime;
        MachineWhellRoot->AddLocalRotation(FRotator(0, 0, 2));

        if (InstallGauge >= 100)
        {
            InstallGauge = 0;
            InstallCount++;
            Character->ItemRemove(EItemType::GADGET, 0);
        }
    }
    else
    {
        if (bIsInstall)
        {
            bIsInstall = false;

            if (Character->State == ECharacterState::MACHINE_INSTALL)
                Character->SetECharacterState(ECharacterState::LAND);

            InstallAudioComponent->FadeOut(0.3f, 0);

            if (InstallCount >= InstallCompleteCount)
            {
                InstallParticleSystem->SetVisibility(false);
                CompleteLight->SetVisibility(true);

                AUnderWorldGameMode* GameMode = Cast<AUnderWorldGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
                GameMode->CompleteMachineInstall();
            }
        }
    }
}

void AMachine::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ASurvivorCharacter* OtherCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (OtherCharacter)
    {
        if (Character == NULL) 
        {
            Character = OtherCharacter;
            Character->OnInputMachineInstall.AddDynamic(this, &AMachine::SetInput);
        }

        bIsOverlap = true;
    }
}

void AMachine::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ASurvivorCharacter* OtherCharacter = Cast<ASurvivorCharacter>(OtherActor);
    if (OtherCharacter)
    {
        bIsOverlap = false;
    }
}

void AMachine::SetInput(bool bIsActive)
{
    bIsInput = bIsOverlap && bIsActive;
}

FTransform AMachine::GetRandomTransform(float Radius)
{
    FNavLocation NavigationLocation;
    UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetRandomReachablePointInRadius(EnemySpawnPoint->GetComponentLocation(), Radius, NavigationLocation);
    return UKismetMathLibrary::MakeTransform(NavigationLocation, UKismetMathLibrary::FindLookAtRotation(NavigationLocation, EnemySpawnPoint->GetComponentLocation()));
}