// Fill out your copyright notice in the Description page of Project Settings.


#include "walkSteelSynthComponent.h"

float walkSteelModesData[30][2] =
{
{312.231445, 1.000000},
{78.057861, 0.579669},
{366.064453, 0.144455},
{417.205811, 0.327398},
{460.272217, 0.064290},
{164.190674, 0.107312},
{645.996094, 0.347686},
{511.413574, 0.058772},
{207.257080, 0.048228},
{826.336670, 0.244489},
{263.781738, 0.080775},
{554.479980, 0.155540},
{1030.902100, 0.058451},
{1205.859375, 0.100832},
{925.927734, 0.031684},
{1394.274902, 0.062470},
{1335.058594, 0.016046},
{597.546387, 0.041296},
{1693.048096, 0.015028},
{1160.101318, 0.006746},
{1754.956055, 0.004415},
{1076.660156, 0.025077},
{2282.519531, 0.015547},
{2188.311768, 0.019796},
{1824.938965, 0.014682},
{1469.641113, 0.020855},
{1886.846924, 0.008658},
{2142.553711, 0.010971},
{3081.939697, 0.007144},
{3359.179688, 0.022861},
};

std::random_device UwalkSteelSynthComponent::rd;
std::default_random_engine UwalkSteelSynthComponent::dre(rd());
std::uniform_real_distribution<float> UwalkSteelSynthComponent::urd(-1.0f, 1.0f);
std::uniform_real_distribution<float> walkSteelRandomGain(1.0f, 3.0f);
const int UwalkSteelSynthComponent::MODES_NUMBER = 30;
const float UwalkSteelSynthComponent::SHORTEST_FREQ = 78.057861f;
const float UwalkSteelSynthComponent::BASE_RELEASE = 400.0f;

bool UwalkSteelSynthComponent::isInitLoaded = false;
maxiSample UwalkSteelSynthComponent::sourceSounds[10];
bool UwalkSteelSynthComponent::isBufferUsed[10];

bool UwalkSteelSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*tempProjectContentPath);
	FString findWavName = "walkMetal.wav";
	std::string filePath = TCHAR_TO_UTF8(*fileFullPath);
	fileFullPath += findWavName;
	filePath = TCHAR_TO_UTF8(*fileFullPath);
	mySample.load(filePath);
	if (!isInitLoaded)
	{
		isInitLoaded = true;
		for (int i = 0; i < 10; ++i)
		{
			sourceSounds[i].load(filePath);
			isBufferUsed[i] = false;
		}
	}
	for (int i = 0; i < 10; ++i)
	{
		if (!isBufferUsed[i])
		{
			currentUsingBuffer = i;
			isBufferUsed[i] = true;
		}
	}
	originEnv.setAttack(1.0f);
	originEnv.setDecay(1.0f);
	originEnv.setSustain(1.0f);
	originEnv.setRelease(BASE_RELEASE);

	for (int i = 0; i < MODES_NUMBER; ++i)
	{
		modesEnv[i].setAttack(1.0f);
		modesEnv[i].setDecay(1.0f);
		modesEnv[i].setSustain(1.0f);
		modesEnv[i].setRelease(BASE_RELEASE / (walkSteelModesData[i][0] / SHORTEST_FREQ));
		if (urd(dre) >= 0.0f)
			fixedGain[i] = walkSteelModesData[i][1] / walkSteelRandomGain(dre);
		else
		{
			fixedGain[i] = walkSteelModesData[i][1] * walkSteelRandomGain(dre);
			if (fixedGain[i] >= 1.0f)
				fixedGain[i] = fixedGain[i] / 3;
		}
		//if (urd(dre) > 0.0f)
		//{
			//fixedGain[i] = walkSteelModesData[i][1] / steelRandomGain(dre);
		//}
		//else
		//{
		//fixedGain[i] = walkSteelModesData[i][1] * walkSteelRandomGain(dre);
		//if (fixedGain[i] > 1.0f) fixedGain[i] = 1.0f;
	//}
	}

	isPlayOnce = true;
	return true;
}

void UwalkSteelSynthComponent::OnEndGenerate()
{
	isBufferUsed[currentUsingBuffer] = false;
	sourceSounds[currentUsingBuffer].reset();
}

int32 UwalkSteelSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
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
		float sourceSoundValue = mySample.play();//sourceSounds[currentUsingBuffer].play();
		float decayVolume[MODES_NUMBER] = { 0, };
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			decayVolume[i] = modesEnv[i].adsr(1, modesEnv[i].trigger);
			out += (originOsc[i].sinewave(walkSteelModesData[i][0]) * walkSteelModesData[i][1]) * decayVolume[i];
		}

		sourceSoundValue *= originEnv.adsr(1, originEnv.trigger);

		auto residual = sourceSoundValue - out;
		auto fixedOut = 0.0f;
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			fixedOut += (fixedOsc[i].sinewave(walkSteelModesData[i][0]) * fixedGain[i]) * decayVolume[i];
		}

		OutAudio[Sample] = (residual + fixedOut) * (0.7f * multiplier);//(residual + fixedOut) * (0.3f * multiplier);
	}

	return NumSamples;
}

void UwalkSteelSynthComponent::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
		{

		});
}