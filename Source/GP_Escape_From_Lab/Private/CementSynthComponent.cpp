// Fill out your copyright notice in the Description page of Project Settings.


#include "CementSynthComponent.h"

float cementModesData[120][2] =
{
	{271.856689, 0.589141},
{721.362305, 0.710240},
{950.152588, 0.746713},
{360.681152, 0.302274},
{586.779785, 0.592631},
{640.612793, 0.416362},
{427.972412, 0.381871},
{1448.107910, 0.552643},
{1491.174316, 0.333468},
{993.218994, 0.359790},
{818.261719, 0.293606},
{541.021729, 0.626679},
{882.861328, 0.476771},
{1717.272949, 0.907333},
{1041.668701, 0.344526},
{1130.493164, 0.518515},
{1851.855469, 0.529587},
{1935.296631, 0.521256},
{1760.339355, 0.429141},
{1205.859375, 0.777867},
{2793.933105, 0.954382},
{4201.666260, 0.318360},
{1655.364990, 0.692264},
{1375.433350, 0.524354},
{2024.121094, 0.756351},
{2196.386719, 0.402105},
{1978.363037, 0.414098},
{2694.342041, 0.914333},
{3886.743164, 0.782005},
{1558.465576, 0.316555},
{2099.487305, 0.356205},
{4005.175781, 0.464835},
{1609.606934, 0.546831},
{2242.144775, 0.461529},
{4879.962158, 1.000000},
{2406.335449, 0.409505},
{2317.510986, 0.798601},
{2150.628662, 0.532705},
{5084.527588, 0.444445},
{2748.175049, 0.412819},
{2855.841064, 0.444465},
{6363.061523, 0.610259},
{3781.768799, 0.798887},
{2492.468262, 0.405141},
{3189.605713, 0.706055},
{4616.180420, 0.240308},
{3243.438721, 0.387546},
{2936.590576, 0.738152},
{3404.937744, 0.196200},
{4107.458496, 0.685943},
{3052.331543, 0.587080},
{4949.945068, 0.597019},
{5321.392822, 0.508722},
{4336.248779, 0.587928},
{3827.526855, 0.546706},
{2645.892334, 0.393156},
{4390.081787, 0.448135},
{3466.845703, 0.532432},
{5784.356689, 0.226443},
{5014.544678, 0.311735},
{4812.670898, 0.373481},
{7310.522461, 0.155999},
{3601.428223, 0.780649},
{3544.903564, 0.434986},
{8457.165527, 0.813370},
{6134.271240, 0.242728},
{5447.900391, 0.239800},
{3644.494629, 0.468774},
{7542.004395, 0.483740},
{7789.636230, 0.625381},
{4155.908203, 0.354697},
{3943.267822, 0.419574},
{4570.422363, 0.313407},
{8220.300293, 0.527016},
{5402.142334, 0.175223},
{4659.246826, 0.491569},
{5127.593994, 0.366920},
{4713.079834, 0.350828},
{7719.653320, 0.219086},
{4268.957520, 0.431525},
{6761.425781, 0.547277},
{6807.183838, 0.202043},
{6594.543457, 0.260155},
{6177.337646, 0.565812},
{8171.850586, 0.636157},
{5684.765625, 0.499967},
{7851.544189, 0.662102},
{7665.820313, 0.207993},
{8360.266113, 0.335959},
{9054.711914, 0.171992},
{7119.415283, 0.135159},
{6850.250244, 0.148988},
{7192.089844, 0.326771},
{8575.598145, 0.368221},
{5937.780762, 0.169233},
{8653.656006, 0.533472},
{5738.598633, 0.384336},
{7259.381104, 0.254149},
{6513.793945, 0.186602},
{6936.383057, 0.198338},
{6715.667725, 0.181714},
{8406.024170, 0.553402},
{6031.988525, 0.210569},
{7899.993896, 0.372129},
{8023.809814, 0.297486},
{7027.899170, 0.094360},
{7614.678955, 0.340612},
{7951.135254, 0.351430},
{6637.609863, 0.278999},
{8804.388428, 0.133301},
{8742.480469, 0.304968},
{8847.454834, 0.212707},
{9568.817139, 0.185743},
{9436.926270, 0.353556},
{9165.069580, 0.140117},
{10333.245850, 0.097385},
{10053.314209, 0.435370},
{11025.000000, 0.076060},
{10704.693604, 0.177282},
{13592.834473, 0.384730},
};

std::random_device UCementSynthComponent::rd;
std::default_random_engine UCementSynthComponent::dre(rd());
std::uniform_real_distribution<float> UCementSynthComponent::urd(1.0f, 2.0f);
const int UCementSynthComponent::MODES_NUMBER = 120;
const float UCementSynthComponent::SHORTEST_FREQ = 271.856689;
const float UCementSynthComponent::BASE_RELEASE = 150.0f;

bool UCementSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = FPaths::ConvertRelativePathToFull(tempProjectContentPath);
	FString findWavName = "Bullet_impact_ceramic.wav";
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

	originEnv.setAttack(120.0f);
	originEnv.setDecay(120.0f);
	originEnv.setSustain(120.0f);
	originEnv.setRelease(BASE_RELEASE);

	for (int i = 0; i < MODES_NUMBER; ++i)
	{
		modesEnv[i].setAttack(1.0f);
		modesEnv[i].setDecay(1.0f);
		modesEnv[i].setSustain(1.0f);
		modesEnv[i].setRelease(BASE_RELEASE / (cementModesData[i][0] / SHORTEST_FREQ));


		if (cementModesData[i][1] > 0.7f)
		{
			fixedGain[i] = cementModesData[i][1] * UCementSynthComponent::urd(UCementSynthComponent::dre);
		}
		else
		{
			fixedGain[i] = cementModesData[i][1] / UCementSynthComponent::urd(UCementSynthComponent::dre);
		}
		
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(SampleRate));
	isPlayOnce = true;
	return true;
}

int32 UCementSynthComponent::OnGenerateAudio(float* OutAudio, int32 NumSamples)
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
			out += (originOsc[i].sinewave(cementModesData[i][0]) * cementModesData[i][1]) * decayVolume[i];
		}

		sourceSoundValue *= originEnv.adsr(1, originEnv.trigger);

		auto residual = sourceSoundValue - out;
		auto fixedOut = 0.0f;
		for (int i = 0; i < MODES_NUMBER; ++i)
		{
			fixedOut += (fixedOsc[i].sinewave(cementModesData[i][0]) * fixedGain[i]) * decayVolume[i];
		}

		OutAudio[Sample] = (fixedOut + residual); //* 0.15f;

		}

	return NumSamples;
}

void UCementSynthComponent::SetFrequency(const float InFrequencyHz)
{
#if SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
	// Use this protected base class method to push a lambda function which will safely execute in the audio render thread.
	SynthCommand([this, InFrequencyHz]()
	{
		Osc.SetFrequency(InFrequencyHz);
		Osc.Update();
	});
#endif // SYNTHCOMPONENT_EX_OSCILATOR_ENABLED
}