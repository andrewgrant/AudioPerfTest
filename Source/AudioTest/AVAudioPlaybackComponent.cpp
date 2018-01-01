// Fill out your copyright notice in the Description page of Project Settings.

#include "AVAudioPlaybackComponent.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Engine/Engine.h"
#import <AVFoundation/AVFoundation.h>

@interface NSFileManager (hidden)
- (NSString*) dumpFilesAt:(NSString*)path recursive:(BOOL)recursive;
@end

@implementation NSFileManager (hidden)
- (NSString*) dumpFilesAt:(NSString*)path recursive:(BOOL)recursive linePrefix:(NSString*)prefix;
{
    NSArray* fileList=[self contentsOfDirectoryAtPath:path error:nil];

    NSString* contents = @"";
    
    if ([fileList count] == 0)
    {
        contents = [prefix stringByAppendingString:@"No files\n"];
    }
    else
    {
        for (NSString* file in fileList)
        {
            contents = [contents stringByAppendingFormat:@"%@%@\n", prefix, file];
            if (recursive)
            {
                NSString* newPrefix = [@"-" stringByAppendingString:prefix];
                NSString* fullPath = [NSString stringWithFormat:@"%@/%@", path, file];
                BOOL isDir = false;
                if ([self fileExistsAtPath:fullPath isDirectory:&isDir] && isDir)
                {
                    NSString* dirContents = [self dumpFilesAt:fullPath recursive:recursive linePrefix:newPrefix];
                    contents = [contents stringByAppendingFormat:@"%@", dirContents];
                }
            }
        }
    }
    
    return contents;
}

- (NSString*) dumpFilesAt:(NSString*)path recursive:(BOOL)recursive
{
    return [self dumpFilesAt:path recursive:recursive linePrefix:@""];
}
@end


struct FAVAudioSoundImpl : public FAVAudioSoundBase
{
    FAVAudioSoundImpl()
    {
        AudioPlayer = nullptr;
    }
    
    ~FAVAudioSoundImpl()
    {
        StopSound();
    }
    
    virtual void StartSound(const TCHAR* Path) override
    {
        /*FString ContentDir = FPaths::ProjectContentDir();
        FString AbsContentPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*ContentDir);
        
        NSString* contentPath = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*AbsContentPath)];
        
        NSString* pathContents = [[NSFileManager  defaultManager] dumpFilesAt:contentPath recursive:YES];
        NSLog(@"contentPath:\n%@", pathContents);
        
        NSString* bundlePath = [[NSBundle mainBundle] resourcePath];
        pathContents = [[NSFileManager  defaultManager] dumpFilesAt:bundlePath recursive:YES];
        NSLog(@"bundleContents:\n%@", pathContents);
        
        NSString* documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
        pathContents = [[NSFileManager  defaultManager] dumpFilesAt:documentPath recursive:YES];
        NSLog(@"docContents:\n%@", pathContents);
        
        //NSArray* bundleFiles=[[NSFileManager defaultManager] contentsOfDirectoryAtPath:bundlePath error:nil];
        //NSLog(@"bundle files: %@",bundleFiles);
        
        NSString* cookedPath = [NSString stringWithFormat:@"%@/cookeddata", bundlePath];
        pathContents = [[NSFileManager  defaultManager] dumpFilesAt:cookedPath recursive:YES];
        NSLog(@"cookeddata contents:\n%@", pathContents);
        
        documentPath = [NSString stringWithFormat:@"%@/AudioTest", documentPath];
        pathContents = [[NSFileManager  defaultManager] dumpFilesAt:documentPath recursive:YES];
        NSLog(@"audioTest contents:\n%@", pathContents);
         */
        
        FString AbsPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(Path);
        NSString* nsPath = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*AbsPath)];
        
        if ([[NSFileManager defaultManager] fileExistsAtPath:nsPath] == NO)
        {
            NSLog(@"No such file %@", nsPath);
            return;
        }

        NSURL *soundUrl = [NSURL fileURLWithPath:nsPath];
        
        // Create audio player object and initialize with URL to sound
        AudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:soundUrl error:nil];
    
        [AudioPlayer play];
    }
    
    virtual void StopSound() override
    {
        [AudioPlayer stop];
    }
    
    virtual bool IsPlaying() const override
    {
        return [AudioPlayer isPlaying];
    }
    
    AVAudioPlayer* AudioPlayer;
};


int32 GetAvailableHardwareDecoders(UInt32 DecoderSpecifier)
{
    int32 numAvailable = 0;
    OSStatus error;
    
    // get an array of AudioClassDescriptions for all installed encoders for the given format
    // the specifier is the format that we are interested in - this is 'aac ' in our case
    UInt32 size;
    
    error = AudioFormatGetPropertyInfo(kAudioFormatProperty_Decoders, sizeof(DecoderSpecifier), &DecoderSpecifier, &size);
    
    if (error)
    {
        NSLog(@"AudioFormatGetPropertyInfo kAudioFormatProperty_Decoders error %d", (int)error);
        return 0;
    }
    
    UInt32 numEntries = size / sizeof(AudioClassDescription);
    AudioClassDescription Descriptions[numEntries];
    
    error = AudioFormatGetProperty(kAudioFormatProperty_Encoders, sizeof(DecoderSpecifier),
                                   &DecoderSpecifier, &size, Descriptions);
    if (error)
    {
        printf("AudioFormatGetProperty kAudioFormatProperty_Decoders error %d", (int)error);
        return 0;
        
    }
    
    for (UInt32 i=0; i < numEntries; ++i)
    {
        if (Descriptions[i].mSubType == DecoderSpecifier /*&&
            Descriptions[i].mManufacturer == kAppleSoftwareAudioCodecManufacturer*/)
        {
            numAvailable++;
        }
    }
    
    return numAvailable;
}

// Sets default values for this component's properties
UAVAudioPlaybackComponent::UAVAudioPlaybackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    int32 HMp3Decoders = GetAvailableHardwareDecoders(kAudioFormatMPEGLayer3);
    int32 HAacDecoders = GetAvailableHardwareDecoders(kAudioFormatMPEG4AAC);
    
    NSLog(@"Number of hardware decoders - MP3:%d, AAC:%d", HMp3Decoders, HAacDecoders);
    
}


// Called when the game starts
void UAVAudioPlaybackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAVAudioPlaybackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAVAudioPlaybackComponent::PlaySound()
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
    
    FString Name = SoundWaves[Index]->GetName();
    Name += TEXT(".mp3");
    
    FString GameContent = FPaths::ProjectContentDir();
    FString Mp3Path = GameContent / TEXT("mp3s") / Name;
    
    TSharedPtr<FAVAudioSoundImpl> NewSound = MakeShareable(new FAVAudioSoundImpl);
    NewSound->StartSound(*Mp3Path);
    
    PlayingSounds.Add(StaticCastSharedPtr<FAVAudioSoundBase>(NewSound));
    
    // force GC now so we have some uninterupted time
    GEngine->ForceGarbageCollection(true);
}


void UAVAudioPlaybackComponent::StopSounds()
{
    for (auto& Sound : PlayingSounds)
    {
        Sound->StopSound();
    }
    
    PlayingSounds.Empty();
}

int32 UAVAudioPlaybackComponent::GetPlayingSoundCount()
{
    int32 Count = 0;
    
    for (auto& Sound : PlayingSounds)
    {
        if (Sound->IsPlaying())
        {
            Count++;
        }
    }
    
    return Count;
}
