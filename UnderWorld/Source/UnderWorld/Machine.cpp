// Fill out your copyright notice in the Description page of Project Settings.


#include "Machine.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SpotLightComponent.h"
#include "Sound/SoundBase.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
    InstallParticleSystem->SetVisibility(true);

    CompleteLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("CompleteLight"));
    CompleteLight->SetupAttachment(RootComponent);
    CompleteLight->SetVisibility(false);
}

void AMachine::BeginPlay()
{
	Super::BeginPlay();	
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

            if (IsValid(InstallAudioComponent))
                InstallAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), InstallSound);
            InstallAudioComponent->FadeIn(0.5f);
        }

        InstallGauge += Character->installSpeed * DeltaTime;
        MachineWhellRoot->AddLocalRotation(FRotator(2, 0, 0));

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

            if (Character->state == ECharacterState::MACHINE_INSTALL)
                Character->SetECharacterState(ECharacterState::LAND);

            InstallAudioComponent->FadeOut(0.3f, 0);

            if (InstallCount >= InstallCompleteCount)
            {
                InstallParticleSystem->SetVisibility(false);
                CompleteLight->SetVisibility(true);
                // 게임모드 Machine Complete 호출
            }
        }
    }
}

void AMachine::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AUnderWorldCharacter* OtherCharacter = Cast<AUnderWorldCharacter>(OtherActor);
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
    AUnderWorldCharacter* OtherCharacter = Cast<AUnderWorldCharacter>(OtherActor);
    if (OtherCharacter)
    {
        bIsOverlap = false;
    }
}

void AMachine::SetInput(bool Active)
{
    bIsInput = Active;
}

FTransform AMachine::GetRandomTransform(float Radius)
{
    //UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(UGameplayStatics::GetPlayerPawn(this->GetWorld(), 0)->GetWorld());
    //FNavLocation NavigationLocation;

    //if (NavigationArea != nullptr)
    //{
    //    NavigationArea->GetRandomReachablePointInRadius(EnemySpawnPoint->GetComponentLocation(), Radius, NavigationLocation);
    //    FTransform RandomTransform;
    //    return UKismetMathLibrary::MakeTransform(NavigationLocation, UKismetMathLibrary::FindLookAtRotation(NavigationLocation, EnemySpawnPoint->GetComponentLocation()));
    //}

    return UKismetMathLibrary::MakeTransform(EnemySpawnPoint->GetComponentLocation(), FRotator());
}