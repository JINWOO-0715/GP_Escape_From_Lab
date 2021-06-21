// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SynthComponent.h"
#include "DSP/Osc.h"
#include "maximilian.h"
#include <random>
#include "walkSteelSynthComponent.generated.h"

// ========================================================================
// UwalkSteelSynthComponent
// Synth component class which implements USynthComponent
// This is a simple hello-world type example which generates a sine-wave
// tone using a DSP oscillator class and implements a single function to set
// the frequency. To enable example:
// 1. Ensure "SignalProcessing" is added to project's .Build.cs in PrivateDependencyModuleNames
// 2. Enable macro below that includes code utilizing SignalProcessing Oscilator
// ========================================================================


UCLASS(ClassGroup = Synth, meta = (BlueprintSpawnableComponent))
class GP_ESCAPE_FROM_LAB_API UwalkSteelSynthComponent : public USynthComponent
{
	GENERATED_BODY()
	
	// Called when synth is created
	virtual bool Init(int32& SampleRate) override;

	// Called to generate more audio
	virtual int32 OnGenerateAudio(float* OutAudio, int32 NumSamples) override;
	virtual void OnEndGenerate() override;

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
	float multiplier = 1.0f;

protected:
	// A simple oscillator class. Can also generate Saw/Square/Tri/Noise.
	bool isPlayOnce;
	float fixedGain[30];
	maxiOsc originOsc[30];
	maxiOsc fixedOsc[30];
	static maxiSample sourceSounds[10];
	maxiSample mySample;
	static bool isInitLoaded;
	static bool isBufferUsed[10];
	maxiEnv modesEnv[30];
	maxiEnv originEnv;
	int currentUsingBuffer;
};