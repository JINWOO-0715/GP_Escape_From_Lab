// Fill out your copyright notice in the Description page of Project Settings.


#include "MySynthComponent.h"

float walkModesData[20][2] =
{
{401.055908, 1.000000},
{139.965820, 0.903084},
{355.297852, 0.812509},
{94.207764, 0.807320},
{309.539795, 0.780818},
{444.122314, 0.760091},
{183.032227, 0.589359},
{487.188721, 0.590386},
{734.820557, 0.503911},
{689.062500, 0.493426},
{777.886963, 0.440752},
{532.946777, 0.433747},
{263.781738, 0.418869},
{586.779785, 0.400621},
{643.304443, 0.336125},
{820.953369, 0.284421},
{1316.217041, 0.197935},
{864.019775, 0.183203},
{1270.458984, 0.174300},
{1391.583252, 0.170874}
};

std::random_device UMySynthComponent::rd;
std::default_random_engine UMySynthComponent::dre(rd());
std::uniform_real_distribution<float> UMySynthComponent::urd(-1.0f,1.0f);
const int UMySynthComponent::MODES_NUMBER = 20;
const float UMySynthComponent::SHORTEST_FREQ = 94.20776f;
const float UMySynthComponent::BASE_RELEASE = 70.0f;
maxiSample UMySynthComponent::sourceSound;
bool UMySynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*tempProjectContentPath);
	FString findWavName = "walk.wav";
	std::string filePath = TCHAR_TO_UTF8(*fileFullPath);
	fileFullPath += findWavName;
	filePath = TCHAR_TO_UTF8(*fileFullPath);
	if (!sourceSound.isReady())
		UMySynthComponent::sourceSound.load(filePath);
	else
		UMySynthComponent::sourceSound.reset();
	originEnv.setAttack(1.0f);
	originEnv.setDecay(1.0f);
	originEnv.setSustain(1.0f);
	originEnv.setRelease(BASE_RELEASE);

	for (int i = 0; i < MODES_NUMBER; ++i)
	{
		modesEnv[i].setAttack(1.0f);
		modesEnv[i].setDecay(1.0f);
		modesEnv[i].setSustain(1.0f);
		modesEnv[i].setRelease(BASE_RELEASE / (walkModesData[i][0] / SHORTEST_FREQ));

		if (UMySynthComponent::urd(UMySynthComponent::dre) > 0.0f)
		{
			fixedGain[i] = walkModesData[i][1] / 1.3;
		}
		else
		{
			fixedGain[i] = walkModesData[i][1] * 1.3;
		}
	}

	isPlayOnce = true;
	return true;
	
}

int32 UMySynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
{
	// Perform DSP operations here
	for (int32 Sample = 0; Sample < NumSamples; ++Sample)
	{
		if (isPlayOnce)
		{
			originEnv.trigger = 1.0f;
			for (int i = 0; i < MODES_NUMBER; ++i)
			{
				modesEnv[i].trigger = 1.0f;
			}
			isPlayOnce = false;
		}
		else
		{
			originEnv.trigger = 0.0f;
			for (int i = 0; i < MODES_NUMBER; ++i)
			{
				modesEnv[i].trigger = 0.0f;
			}
		}
		float out = 0.0f;
		float sourceSoundValue = sourceSound.play();
		float decayVolume[MODES_NUMBER] = { 0, };
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			decayVolume[i] = modesEnv[i].adsr(1, modesEnv[i].trigger);
			out += (originOsc[i].sinewave(walkModesData[i][0]) * walkModesData[i][1]) * decayVolume[i];
		}

		sourceSoundValue *= originEnv.adsr(1, originEnv.trigger);

		auto residual = sourceSoundValue - out;
		auto fixedOut = 0.0f;
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			fixedOut += (fixedOsc[i].sinewave(walkModesData[i][0]) * fixedGain[i]) * decayVolume[i];
		}
		
		OutAudio[Sample] = (fixedOut + residual) * 2.0f * multiplier;

	}
	
	return NumSamples;
}

void UMySynthComponent::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
		{

		});
}