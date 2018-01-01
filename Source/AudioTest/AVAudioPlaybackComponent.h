// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundWave.h"
#include "AVAudioPlaybackComponent.generated.h"


struct FAVAudioSoundBase
{
    virtual ~FAVAudioSoundBase() {}
    
    virtual void StartSound(const TCHAR* Path) = 0;
    
    virtual void StopSound() = 0;
    
    virtual bool IsPlaying() const = 0;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUDIOTEST_API UAVAudioPlaybackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAVAudioPlaybackComponent();
    
    UPROPERTY(EditAnywhere)
    TArray<USoundWave*>     SoundWaves;
    
    UFUNCTION(BlueprintCallable)
    void PlaySound();
    
    UFUNCTION(BlueprintCallable)
    void StopSounds();
    
    UFUNCTION(BlueprintCallable)
    int32 GetPlayingSoundCount();
    
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
    
    TArray<TSharedPtr<FAVAudioSoundBase>> PlayingSounds;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
