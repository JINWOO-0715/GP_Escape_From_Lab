// Fill out your copyright notice in the Description page of Project Settings.


#include "SteelSynthComponent.h"

float steelModesData[20][2] =
{
	{2131.787109, 0.717345},
{1528.857422, 1.000000},
{2705.108643, 0.434843},
{2532.843018, 0.364351},
{1878.771973, 0.344835},
{1367.358398, 0.314361},
{2086.029053, 0.185003},
{2174.853516, 0.255966},
{944.769287, 0.071836},
{1009.368896, 0.069623},
{3413.012695, 0.138965},
{3582.586670, 0.184700},
{2877.374268, 0.159833},
{2793.933105, 0.191256},
{2217.919922, 0.029339},
{6110.046387, 0.131946},
{1728.039551, 0.066898},
{4145.141602, 0.076191},
{5205.651855, 0.130939},
{3989.025879, 0.095731},
};

std::random_device USteelSynthComponent::rd;
std::default_random_engine USteelSynthComponent::dre(rd());
std::uniform_real_distribution<float> USteelSynthComponent::urd(-1.0f, 1.0f);
std::uniform_real_distribution<float> steelRandomGain(1.0f, 10.0f);
const int USteelSynthComponent::MODES_NUMBER = 20;
const float USteelSynthComponent::SHORTEST_FREQ = 944.769287f;
const float USteelSynthComponent::BASE_RELEASE = 400.0f;
maxiSample USteelSynthComponent::sourceSound;
bool USteelSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*tempProjectContentPath);
	FString findWavName = "steel.wav";
	std::string filePath = TCHAR_TO_UTF8(*fileFullPath);
	fileFullPath += findWavName;
	filePath = TCHAR_TO_UTF8(*fileFullPath);
	if (!sourceSound.isReady())
		sourceSound.load(filePath);
	else
		sourceSound.reset();
	originEnv.setAttack(400.0f);
	originEnv.setDecay(1.0f);
	originEnv.setSustain(1.0f);
	originEnv.setRelease(BASE_RELEASE-300);

	for (int i = 0; i < MODES_NUMBER; ++i)
	{
		modesEnv[i].setAttack(0.1f);
		modesEnv[i].setDecay(1.0f);
		modesEnv[i].setSustain(1.0f);
		modesEnv[i].setRelease(BASE_RELEASE / (steelModesData[i][0] / SHORTEST_FREQ));

		//if (urd(dre) > 0.0f)
		//{
			//fixedGain[i] = steelModesData[i][1] / steelRandomGain(dre);
		//}
		//else
		//{
			fixedGain[i] = steelModesData[i][1] * steelRandomGain(dre);
			//if (fixedGain[i] > 1.0f) fixedGain[i] = 1.0f;
		//}
	}

	isPlayOnce = true;
	return true;

}

int32 USteelSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
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
			out += (originOsc[i].sinewave(steelModesData[i][0]) * steelModesData[i][1]) * decayVolume[i];
		}

		sourceSoundValue *= originEnv.adsr(1, originEnv.trigger);

		auto residual = sourceSoundValue - out;
		auto fixedOut = 0.0f;
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			fixedOut += (fixedOsc[i].sinewave(steelModesData[i][0]) * fixedGain[i]) * decayVolume[i];
		}

		OutAudio[Sample] = (residual+fixedOut) * 0.3f;
	}

	return NumSamples;
}

void USteelSynthComponent::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
		{

		});
}