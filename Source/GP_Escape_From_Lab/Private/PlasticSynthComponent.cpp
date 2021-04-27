// Fill out your copyright notice in the Description page of Project Settings.


#include "PlasticSynthComponent.h"

float plasticModesData[60][2] =
{
{1022.827148, 1.000000},
{374.139404, 0.624444},
{616.387939, 0.493763},
{180.340576, 0.639704},
{915.161133, 0.828340},
{1184.326172, 0.794608},
{255.706787, 0.520302},
{1135.876465, 0.716544},
{417.205811, 0.657833},
{1413.116455, 0.812274},
{724.053955, 0.697105},
{1954.138184, 0.467226},
{1230.084229, 0.577432},
{659.454346, 0.600003},
{1545.007324, 0.281210},
{869.403076, 0.706088},
{495.263672, 0.650817},
{802.111816, 0.624149},
{1308.142090, 0.487560},
{1084.735107, 0.641253},
{304.156494, 0.410859},
{570.629883, 0.367298},
{1590.765381, 0.462031},
{1752.264404, 0.359942},
{977.069092, 0.427748},
{1701.123047, 0.505146},
{1835.705566, 0.415309},
{1881.463623, 0.181287},
{2282.519531, 0.476888},
{1499.249268, 0.291029},
{1997.204590, 0.408616},
{2567.834473, 0.439270},
{2166.778564, 0.350337},
{2112.945557, 0.283674},
{1367.358398, 0.359808},
{2376.727295, 0.361251},
{2627.050781, 0.335838},
{2697.033691, 0.153428},
{2863.916016, 0.123221},
{2446.710205, 0.204614},
{3038.873291, 0.256080},
{3534.136963, 0.082999},
{3232.672119, 0.184543},
{2500.543213, 0.175732},
{2793.933105, 0.265161},
{3170.764160, 0.145404},
{3466.845703, 0.131028},
{3281.121826, 0.161302},
{3380.712891, 0.190815},
{3108.856201, 0.207611},
{4893.420410, 0.037953},
{3814.068604, 0.186571},
{2987.731934, 0.212714},
{3668.719482, 0.109023},
{3711.785889, 0.083069},
{3582.586670, 0.046543},
{5429.058838, 0.064817},
{4952.636719, 0.034985},
{14061.181641, 0.043371},
{14015.423584, 0.025404},
};

std::random_device UPlasticSynthComponent::rd;
std::default_random_engine UPlasticSynthComponent::dre(rd());
std::uniform_real_distribution<float> UPlasticSynthComponent::urd(-1.0f, 1.0f);
uniform_real_distribution<float> urdFixed(3.0f, 10.0f);
const int UPlasticSynthComponent::MODES_NUMBER = 60;
const float UPlasticSynthComponent::SHORTEST_FREQ = 180.340576;
const float UPlasticSynthComponent::BASE_RELEASE = 300.0f;

bool UPlasticSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = FPaths::ConvertRelativePathToFull(tempProjectContentPath);
	FString findWavName = "Bullet_Plastic_Impact.wav";
	std::string filePath = TCHAR_TO_UTF8(*fileFullPath);
	fileFullPath += findWavName;
	filePath = TCHAR_TO_UTF8(*fileFullPath);
	bool isSuccess = sourceSound.load(filePath);
	if (isSuccess)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Load Success");
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Load failed");
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, fileFullPath);
	}

	originEnv.setAttack(1.0f);
	originEnv.setDecay(1.0f);
	originEnv.setSustain(1.0f);
	originEnv.setRelease(BASE_RELEASE*3);

	for (int i = 0; i < MODES_NUMBER; ++i)
	{
		modesEnv[i].setAttack(1.0f);
		modesEnv[i].setDecay(1.0f);
		modesEnv[i].setSustain(1.0f);
		modesEnv[i].setRelease(BASE_RELEASE / (plasticModesData[i][0] / SHORTEST_FREQ));

		//if (UPlasticSynthComponent::urd(UPlasticSynthComponent::dre) > 0.0f)
		//{
			//fixedGain[i] = plasticModesData[i][1] / urdFixed(UPlasticSynthComponent::dre);
		//}
		//else
		//{
			fixedGain[i] = plasticModesData[i][1] * urdFixed(UPlasticSynthComponent::dre);
		//}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(SampleRate));
	isPlayOnce = true;
	return true;

}

int32 UPlasticSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
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
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "envelope on");
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
			out += (originOsc[i].sinewave(plasticModesData[i][0]) * plasticModesData[i][1]) * decayVolume[i];
		}

		sourceSoundValue *= originEnv.adsr(1, originEnv.trigger);
		
		auto residual = sourceSoundValue - out;
		auto fixedOut = 0.0f;
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			fixedOut += (fixedOsc[i].sinewave(plasticModesData[i][0]) * fixedGain[i]) * decayVolume[i];
		}
		
		OutAudio[Sample] = (fixedOut + residual) * 0.15f;

	}

	return NumSamples;
}

void UPlasticSynthComponent::SetFrequency(const float InFrequencyHz)
{
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
		{

		});
}