// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "DSP/Osc.h"
#include "maximilian.h"
#include <random>
#include "WoodSynthComponent.generated.h"

// ========================================================================
// UWoodSynthComponent
// Synth component class which implements USynthComponent
// This is a simple hello-world type example which generates a sine-wave
// tone using a DSP oscillator class and implements a single function to set
// the frequency. To enable example:
// 1. Ensure "SignalProcessing" is added to project's .Build.cs in PrivateDependencyModuleNames
// 2. Enable macro below that includes code utilizing SignalProcessing Oscilator
// ========================================================================

#define SYNTHCOMPONENT_EX_OSCILATOR_ENABLED 0

UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class GP_ESCAPE_FROM_LAB_API UWoodSynthComponent : public USynthComponent
{
	GENERATED_BODY()
	
	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;

	// Sets the oscillator's frequency
	UFUNCTION(BlueprintCallable, Category = "Synth|Components|Audio")
	void SetFrequency(const float FrequencyHz = 440.0f);

public:
	static const int MODES_NUMBER;
	static const float SHORTEST_FREQ;
	static const float BASE_RELEASE;
	static std::random_device rd;
	static std::default_random_engine dre;
	static std::uniform_real_distribution<float> urd;
	static std::uniform_real_distribution<float> urdFixed;
	float multiplier = 1.0f;

protected:
	// A simple oscillator class. Can also generate Saw/Square/Tri/Noise.
	bool isPlayOnce;
	float fixedGain[239];
	maxiOsc originOsc[239];
	maxiOsc fixedOsc[239];
	maxiSample sourceSound;
	maxiEnv modesEnv[239];
	maxiEnv originEnv;
};