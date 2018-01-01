// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultAudioPlaybackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "AudioDevice.h"

// Sets default values for this component's properties
UDefaultAudioPlaybackComponent::UDefaultAudioPlaybackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDefaultAudioPlaybackComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UDefaultAudioPlaybackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDefaultAudioPlaybackComponent::PlaySound()
{
    if (SoundWaves.Num() == 0)
    {
        return;
    }
    
    int32 Index = GetPlayingSoundCount() % SoundWaves.Num();
    
    if (SoundWaves[Index] == nullptr)
    {
        return;
    }

    UGameplayStatics::PlaySound2D(GetOwner(), SoundWaves[Index], 1.0, 1.0, 0, nullptr, GetOwner());
    PlayingSoundCount++;
    
    // force GC now so we have some uninterupted time
    GEngine->ForceGarbageCollection(true);
}

int32 UDefaultAudioPlaybackComponent::GetPlayingSoundCount()
{
#if 1
    return PlayingSoundCount;
#else
    UWorld* World = GetOwner()->GetWorld();
  
    if (World)
    {
        if (FAudioDevice* AudioDevice = World->GetAudioDevice())
        {
            return AudioDevice->GetActiveSounds().Num();
        }
    }
    
    return 0;
#endif
}

void UDefaultAudioPlaybackComponent::StopSounds()
{
    UWorld* World = GetOwner()->GetWorld();
    if (!World)
    {
        return;
    }
    
    if (FAudioDevice* AudioDevice = World->GetAudioDevice())
    {
        AudioDevice->StopAllSounds(true);
    }
    
    PlayingSoundCount = 0;
}


