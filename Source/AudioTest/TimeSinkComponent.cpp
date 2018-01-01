// Fill out your copyright notice in the Description page of Project Settings.

#include "TimeSinkComponent.h"
#include "RenderingThread.h"


// Sets default values for this component's properties
UTimeSinkComponent::UTimeSinkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTimeSinkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTimeSinkComponent::BurnTime(const int32 TimeMS)
{
    const double TimeNow = FPlatformTime::Seconds();
    
    do
    {
        const double ElapsedMS = (FPlatformTime::Seconds() - TimeNow) * 1000;
        
        if (ElapsedMS >= TimeMS)
        {
            break;
        }
        
    } while(true);
}


// Called every frame
void UTimeSinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
    
    if (bEnabled)
    {
        ENQUEUE_RENDER_COMMAND(BurnGPUTimeCmd)(
              [this](FRHICommandListImmediate& RHICmdList)
              {
                  BurnTime(GPUBurnTimeInMS);
              }
        );
        
        BurnTime(CPUBurnTimeInMS);
    }
}

