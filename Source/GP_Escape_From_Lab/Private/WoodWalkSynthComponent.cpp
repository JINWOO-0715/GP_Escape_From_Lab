// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodWalkSynthComponent.h"

float walkWoodModesData[30][2] =
{
{193.798828, 0.509796},
{551.788330, 0.207421},
{75.366211, 1.000000},
{288.006592, 0.344973},
{734.820557, 0.204596},
{436.047363, 0.123761},
{500.646973, 0.218596},
{331.072998, 0.134960},
{148.040771, 0.360399},
{236.865234, 0.201767},
{602.929688, 0.176737},
{777.886963, 0.307634},
{374.139404, 0.259830},
{29.608154, 0.345144},
{654.071045, 0.064528},
{831.719971, 0.250525},
{925.927734, 0.173302},
{1135.876465, 0.024441},
{1030.902100, 0.091186},
{985.144043, 0.105661},
{1090.118408, 0.092655},
{1178.942871, 0.063303},
{1372.741699, 0.056834},
{1326.983643, 0.059046},
{1235.467529, 0.051652},
{1418.499756, 0.045078},
{1281.225586, 0.030031},
{880.169678, 0.060001},
{1464.257813, 0.017247},
{1510.015869, 0.016018}
};

std::random_device UWoodWalkSynthComponent::rd;
std::default_random_engine UWoodWalkSynthComponent::dre(rd());
std::uniform_real_distribution<float> UWoodWalkSynthComponent::urd(-1.0f, 1.0f);
std::uniform_real_distribution<float> walkWoodRandomGain(1.0f, 2.0f);
const int UWoodWalkSynthComponent::MODES_NUMBER = 30;
const float UWoodWalkSynthComponent::SHORTEST_FREQ = 29.608154f;
const float UWoodWalkSynthComponent::BASE_RELEASE = 500.0f;

bool UWoodWalkSynthComponent::isInitLoaded = false;
maxiSample UWoodWalkSynthComponent::sourceSounds[10];
bool UWoodWalkSynthComponent::isBufferUsed[10];

bool UWoodWalkSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*tempProjectContentPath);
	FString findWavName = "walkWood.wav";
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
		modesEnv[i].setRelease(BASE_RELEASE / (walkWoodModesData[i][0] / SHORTEST_FREQ));
		if (urd(dre) >= 0.0f)
			fixedGain[i] = walkWoodModesData[i][1] / walkWoodRandomGain(dre);
		else
		{
			fixedGain[i] = walkWoodModesData[i][1] * walkWoodRandomGain(dre);
			if (fixedGain[i] >= 1.0f)
				fixedGain[i] = fixedGain[i] / 2;
		}
	}

	isPlayOnce = true;
	return true;
}

int32 UWoodWalkSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
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
			out += (originOsc[i].sinewave(walkWoodModesData[i][0]) * walkWoodModesData[i][1]) * decayVolume[i];
		}

		sourceSoundValue *= (originEnv.adsr(1, originEnv.trigger) * 2);
		
		auto residual = sourceSoundValue - out;
		auto fixedOut = 0.0f;
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			fixedOut += (fixedOsc[i].sinewave(walkWoodModesData[i][0]) * fixedGain[i]) * decayVolume[i];
		}

		OutAudio[Sample] = (residual + fixedOut) * (multiplier);
	}

	return NumSamples;
}

void UWoodWalkSynthComponent::SetFrequency(const float InFrequencyHz)
{
	SynthCommand([this, InFrequencyHz]()
	{

	});
}