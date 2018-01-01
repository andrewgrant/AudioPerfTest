// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TimeSinkComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AUDIOTEST_API UTimeSinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTimeSinkComponent();
    
    UPROPERTY(EditAnywhere)
    int32 CPUBurnTimeInMS;

    UPROPERTY(EditAnywhere)
    int32 GPUBurnTimeInMS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NumBurns;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxBurns;
    
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // spins for TimeMs milliseconds on the current thread
    static void BurnTime(const int32 TimeMS);
	
};
