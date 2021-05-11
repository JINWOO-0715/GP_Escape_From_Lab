// Fill out your copyright notice in the Description page of Project Settings.


#include "CementSynthComponent.h"

float cementModesData[120][2] =
{
	{721.362305, 0.642136},
{360.681152, 0.273141},
{950.152588, 0.686199},
{586.779785, 0.537387},
{640.612793, 0.370403},
{1448.107910, 0.502153},
{993.218994, 0.326527},
{818.261719, 0.266060},
{1491.174316, 0.304546},
{882.861328, 0.431835},
{1717.272949, 0.804874},
{1041.668701, 0.312846},
{1130.493164, 0.434692},
{1889.538574, 0.733544},
{1205.859375, 0.707642},
{4201.666260, 0.314878},
{1935.296631, 0.446358},
{2793.933105, 0.863376},
{1760.339355, 0.394054},
{1658.056641, 0.616630},
{1843.780518, 0.332016},
{2024.121094, 0.652338},
{3886.743164, 0.745137},
{2694.342041, 0.819494},
{2196.386719, 0.357288},
{4879.962158, 0.996180},
{6363.061523, 0.666530},
{4005.175781, 0.443483},
{1978.363037, 0.356742},
{2099.487305, 0.327836},
{2242.144775, 0.387130},
{5084.527588, 0.458222},
{2409.027100, 0.373517},
{2317.510986, 0.743019},
{2150.628662, 0.492580},
{2748.175049, 0.393550},
{3189.605713, 0.695836},
{4616.180420, 0.243832},
{3781.768799, 0.733409},
{2855.841064, 0.420687},
{2492.468262, 0.375530},
{2936.590576, 0.724601},
{8457.165527, 1.000000},
{3243.438721, 0.344880},
{3404.937744, 0.170610},
{4107.458496, 0.642995},
{4949.945068, 0.620110},
{5321.392822, 0.462237},
{7789.636230, 0.748618},
{3052.331543, 0.536309},
{7310.522461, 0.188579},
{8220.300293, 0.644938},
{5784.356689, 0.223757},
{4336.248779, 0.569700},
{7542.004395, 0.569387},
{4390.081787, 0.451314},
{3827.526855, 0.532356},
{6134.271240, 0.264563},
{5014.544678, 0.296806},
{9054.711914, 0.229189},
{3466.845703, 0.507857},
{8171.850586, 0.770279},
{4812.670898, 0.345079},
{7851.544189, 0.792432},
{5450.592041, 0.287332},
{8650.964355, 0.690338},
{8360.266113, 0.424458},
{7719.653320, 0.291053},
{6761.425781, 0.611254},
{6594.543457, 0.285904},
{5404.833984, 0.158271},
{4570.422363, 0.307759},
{7665.820313, 0.244950},
{8403.332520, 0.721635},
{8575.598145, 0.427859},
{6177.337646, 0.618348},
{4713.079834, 0.354085},
{6807.183838, 0.221822},
{5127.593994, 0.386933},
{5682.073975, 0.540031},
{7897.302246, 0.512866},
{9436.926270, 0.516146},
{7192.089844, 0.386326},
{6850.250244, 0.170374},
{7119.415283, 0.159748},
{8844.763184, 0.272089},
{10053.314209, 0.611029},
{8021.118164, 0.347419},
{9593.041992, 0.323547},
{7259.381104, 0.281575},
{9649.566650, 0.396126},
{7614.678955, 0.400628},
{6936.383057, 0.209451},
{6513.793945, 0.207986},
{8739.788818, 0.383334},
{7956.518555, 0.429172},
{6715.667725, 0.206090},
{9786.840820, 0.300345},
{6422.277832, 0.430700},
{9692.633057, 0.342272},
{6637.609863, 0.304440},
{7030.590820, 0.096020},
{8126.092529, 0.521249},
{9945.648193, 0.534769},
{9165.069580, 0.186522},
{8799.005127, 0.234781},
{7358.972168, 0.396380},
{9337.335205, 0.322772},
{8987.420654, 0.450854},
{9224.285889, 0.190093},
{8524.456787, 0.391225},
{10330.554199, 0.092577},
{9741.082764, 0.371853},
{10174.438477, 0.148252},
{9113.928223, 0.291475},
{9872.973633, 0.286741},
{10704.693604, 0.212882},
{10572.802734, 0.351172},
{10260.571289, 0.308205},
{10403.228760, 0.316470}
};

std::random_device UCementSynthComponent::rd;
std::default_random_engine UCementSynthComponent::dre(rd());
std::uniform_real_distribution<float> UCementSynthComponent::urd(1.0f, 2.0f);
const int UCementSynthComponent::MODES_NUMBER = 120;
const float UCementSynthComponent::SHORTEST_FREQ = 360.681152;
const float UCementSynthComponent::BASE_RELEASE = 550.0f;

bool UCementSynthComponent::Init(int32& SampleRate)
{
	NumChannels = 1;
	FString tempProjectContentPath = FPaths::ProjectContentDir();
	FString fileFullPath = FPaths::ConvertRelativePathToFull(tempProjectContentPath);
	FString findWavName = "Bullet_impact_ceramic.wav";
	std::string filePath = TCHAR_TO_UTF8(*fileFullPath);
	fileFullPath += findWavName;
	filePath = TCHAR_TO_UTF8(*fileFullPath);

	sourceSound.load(filePath);
	
	originEnv.setAttack(10.0f);
	originEnv.setDecay(10.0f);
	originEnv.setSustain(80.0f);
	originEnv.setRelease(BASE_RELEASE);

	for (int i = 0; i < MODES_NUMBER; ++i)
	{
		modesEnv[i].setAttack(1.0f);
		modesEnv[i].setDecay(1.0f);
		modesEnv[i].setSustain(1.0f);
		modesEnv[i].setRelease(BASE_RELEASE / (cementModesData[i][0] / SHORTEST_FREQ)/2.1f);


		if (cementModesData[i][1] > 0.6f)
		{
			fixedGain[i] = cementModesData[i][1] / UCementSynthComponent::urd(UCementSynthComponent::dre);
		}
		else
		{
			fixedGain[i] = cementModesData[i][1] * UCementSynthComponent::urd(UCementSynthComponent::dre);
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

		OutAudio[Sample] = (fixedOut + residual) * 1.2f; //* 0.15f;

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