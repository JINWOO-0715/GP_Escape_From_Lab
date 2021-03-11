#include "SynthesizedSound.h"
#include <random>

std::random_device rd;
std::default_random_engine dre(rd());
std::uniform_real_distribution<float> urd(-1.0, 1.0);

maxiOsc tempOSc;

float SynthesizedSound::steelModesData[MAX_MODES][2]=
{
	{2314.819336, 1.000000},
{812.878418, 0.823778},
{944.769287, 0.433846},
{1695.739746, 0.514097},
{2409.027100, 0.270281},
{4376.623535, 0.246539},
{1787.255859, 0.203437},
{1370.050049, 0.205115},
{2880.065918, 0.243906},
{2586.676025, 0.237636},
{1555.773926, 0.167016},
{1012.060547, 0.176879},
{2083.337402, 0.166705},
{2269.061279, 0.136931},
{885.552979, 0.173297},
{1601.531982, 0.184145},
{1313.525391, 0.204968},
{2707.800293, 0.180958},
{2500.543213, 0.183414},
{1649.981689, 0.154792},
{1738.806152, 0.153796},
{1835.705566, 0.182304},
{1254.309082, 0.194206},
{1884.155273, 0.127782},
{2126.403809, 0.127800},
{3811.376953, 0.136344},
{7603.912354, 0.187460},
{2767.016602, 0.155212},
{2834.307861, 0.164529},
{4441.223145, 0.166955},
{3719.860840, 0.131437},
{4624.255371, 0.138187},
{7385.888672, 0.167439},
{2357.885742, 0.141182},
{3889.434814, 0.112765},
{6753.350830, 0.157187},
{3633.728027, 0.114635},
{2010.662842, 0.116862},
{3544.903564, 0.135484},
{4516.589355, 0.144767},
{7232.464600, 0.150572},
{3972.875977, 0.113167},
{2950.048828, 0.096263},
{2662.042236, 0.097302},
{3192.297363, 0.104709},
{6861.016846, 0.139377},
{4578.497314, 0.133586},
{7130.181885, 0.125318},
{4721.154785, 0.128275},
{6957.916260, 0.129374},
{7746.569824, 0.129480},
{4120.916748, 0.123518},
{8637.506104, 0.120828},
{7006.365967, 0.099468},
{9111.236572, 0.130108},
{3469.537354, 0.127335},
{6201.562500, 0.115451},
{4217.816162, 0.111732},
{3410.321045, 0.104982},
{5348.309326, 0.118194},
{7509.704590, 0.119777},
{3003.881836, 0.100943},
{7173.248291, 0.114921},
{4885.345459, 0.094400},
{4163.983154, 0.099472},
{5041.461182, 0.105405},
{4050.933838, 0.083506},
{3281.121826, 0.088927},
{5216.418457, 0.106982},
{9393.859863, 0.108130},
{3057.714844, 0.076978},
{6319.995117, 0.102792},
{6513.793945, 0.095312},
{5396.759033, 0.088521},
{3359.179688, 0.078918},
{7959.210205, 0.098831},
{6680.676270, 0.101610},
{4928.411865, 0.079377},
{8572.906494, 0.092848},
{12564.624023, 0.096102},
{9170.452881, 0.097496},
{4783.062744, 0.079629},
{5447.900391, 0.068387},
{3765.618896, 0.077038},
{7646.978760, 0.086257},
{5542.108154, 0.093064},
{6031.988525, 0.086248},
{5170.660400, 0.085377},
{6274.237061, 0.072754},
{5092.602539, 0.082411},
{8796.313477, 0.078193},
{7838.085938, 0.076079},
{5593.249512, 0.074364},
{10710.076904, 0.081558},
{7908.068848, 0.078746},
{8327.966309, 0.073511},
{7049.432373, 0.076435},
{8944.354248, 0.077361},
{6912.158203, 0.073383},
{9910.656738, 0.076351},
{8850.146484, 0.076761},
{6459.960938, 0.057528},
{8128.784180, 0.066352},
{6796.417236, 0.066600},
{9593.041992, 0.065733},
{8702.105713, 0.069053},
{8048.034668, 0.071024},
{7690.045166, 0.047387},
{8476.007080, 0.069547},
{9547.283936, 0.050238},
{12489.257813, 0.063309},
{8239.141846, 0.064538},
{9278.118896, 0.062698},
{8403.332520, 0.063476},
{9458.459473, 0.063474},
{9727.624512, 0.056622},
{9065.478516, 0.047411},
{10575.494385, 0.053117},
{11423.364258, 0.054588},
{14007.348633, 0.036473}
};
float SynthesizedSound::walkModesData[MAX_MODES][2];
float SynthesizedSound::plasticModesData[MAX_MODES][2]=
{
{387.597656, 0.981703},
{896.319580, 1.000000},
{850.561523, 0.700905},
{554.479980, 0.556620},
{966.302490, 0.655078},
{142.657471, 0.337468},
{242.248535, 0.680713},
{481.805420, 0.747712},
{341.839600, 0.796666},
{624.462891, 0.586159},
{1978.363037, 0.038713},
{1528.857422, 0.219886},
{1052.435303, 0.233980},
{1372.741699, 0.103569},
{1606.915283, 0.283385},
{667.529297, 0.325687},
{1816.864014, 0.272704},
{1921.838379, 0.116776},
{1695.739746, 0.138930},
{1240.850830, 0.312865},
{2134.478760, 0.308043},
{1415.808105, 0.423125},
{1098.193359, 0.272447},
{1195.092773, 0.086765},
{2287.902832, 0.114794},
{804.803467, 0.203100},
{1757.647705, 0.241262},
{1649.981689, 0.053380},
{753.662109, 0.323599},
{1283.917236, 0.378654},
{2330.969238, 0.078783},
{2021.429443, 0.051203},
{1461.566162, 0.174833},
{2064.495850, 0.026218},
{1859.930420, 0.060630},
{2594.750977, 0.068748},
{1143.951416, 0.196571},
{3596.044922, 0.021190},
{3060.406494, 0.021701},
{2826.232910, 0.070008},
{4161.291504, 0.038855},
{3324.188232, 0.020217},
{2637.817383, 0.036487},
{3119.622803, 0.038769},
{2732.025146, 0.024982},
{2374.035645, 0.041329},
{3003.881836, 0.017691},
{3639.111328, 0.039602},
{3738.702393, 0.067003},
{3865.209961, 0.013423},
{2898.907471, 0.035753},
{4869.195557, 0.026381},
{3450.695801, 0.043811},
{3515.295410, 0.032487},
{5079.144287, 0.031656},
{3910.968018, 0.026147},
{5501.733398, 0.018943},
{4949.945068, 0.026285},
{5348.309326, 0.027712},
{6010.455322, 0.011952}
};

float SynthesizedSound::SHORTEST_FREQS[3];
float SynthesizedSound::BASE_RELEASES[3];
float SynthesizedSound::MODES_NUMS[3];

bool SynthesizedSound::isInitOnce = false;

SynthesizedSound::SynthesizedSound()
{
	if (!isInitOnce)
	{
		isInitOnce = true;

		SHORTEST_FREQS[(int)(WhichSound::STEEL)] = 812.878418;
		BASE_RELEASES[(int)(WhichSound::STEEL)] = 450;
		MODES_NUMS[(int)(WhichSound::STEEL)] = 120;		
		SHORTEST_FREQS[(int)(WhichSound::PLASTIC)] = 142.657471;
		BASE_RELEASES[(int)(WhichSound::PLASTIC)] = 200;//이 값은 실험으로 다시 결정해야함.
		MODES_NUMS[(int)(WhichSound::PLASTIC)] = 60;
		SHORTEST_FREQS[(int)(WhichSound::WALK)] = 94.207764;;
		BASE_RELEASES[(int)(WhichSound::WALK)] = 70;
		MODES_NUMS[(int)(WhichSound::WALK)] = 20;

	}
}

float SynthesizedSound::play()
{
// 	++tick;

	
	if (isPlaying)
	{

		++tick;
 		auto out = 0.0f;
 		auto playValue = sourceSound.play();
 		float decayVolume[MAX_MODES] = { 0, };
 		for (int i = 0; i < modesNumber; ++i)
 		{
 			decayVolume[i] = modesEnv[i].adsr(1, modesEnv[i].trigger);
 
 			out += (originOsc[i].sinewave(steelModesData[i][0]) * steelModesData[i][1]) * decayVolume[i];
 		}
 		playValue *= originEnv.adsr(1, originEnv.trigger);
 		auto residual = playValue - out;
 		auto fixedOut = 0.0f;
 		for (int i = 0; i < modesNumber; ++i)
 		{
 			fixedOut += (fixedOsc[i].sinewave(steelModesData[i][0]) * fixedGain[i]) * decayVolume[i];
 		}
 		
 		if (isPlayOnce)
 		{
 			originEnv.trigger = 0.0f;
 			for (int i = 0; i < modesNumber; ++i)
 			{
 				modesEnv[i].trigger = 0.0f;
 			}
 			isPlayOnce = false;
 		}
		if (tick > 50000)
		{
			isPlaying = false;
			sourceSound.clear();
			tick = 0;
		}
		return fixedOut+residual;
	}
	else
		return 0.0f;

	
}


void SynthesizedSound::initSoundData(WhichSound whichSound)
{
	isPlaying = true;
	isPlayOnce = true;

	switch (whichSound)
	{
	case WhichSound::STEEL:
		modesNumber = MODES_NUMS[int(whichSound)];
		sourceSound.load("/Game/steel.wav");
		originEnv.setAttack(1.0f);
		originEnv.setDecay(1);
		originEnv.setSustain(1);
		originEnv.setRelease(250.0f);
		originEnv.trigger = 1.0f;
		
		for (int i = 0; i < modesNumber; ++i)
		{
			modesEnv[i].setAttack(1.0f);
			modesEnv[i].setDecay(1.0f);
			modesEnv[i].setSustain(1.0f);
			modesEnv[i].setRelease(BASE_RELEASES[int(whichSound)] / (steelModesData[i][0] / SHORTEST_FREQS[int(whichSound)]));

			if (urd(dre) > 0.0f)
				fixedGain[i] = steelModesData[i][1] / 2;
			else
				fixedGain[i] = steelModesData[i][1] * 2;
			modesEnv[i].trigger = 1.0f;
		}
		break;
	case WhichSound::WALK:
		modesNumber = MODES_NUMS[int(whichSound)];
		sourceSound.load("C:/Users/user/Desktop/GP_Escape_From_Lab/Content/walk.wav");
		originEnv.setAttack(1.0f);
		originEnv.setDecay(1);
		originEnv.setSustain(1);
		originEnv.setRelease(70.0f);
		originEnv.trigger = 1.0f;

		for (int i = 0; i < modesNumber; ++i)
		{
			modesEnv[i].setAttack(1.0f);
			modesEnv[i].setDecay(1.0f);
			modesEnv[i].setSustain(1.0f);
			modesEnv[i].setRelease(BASE_RELEASES[int(whichSound)] / (walkModesData[i][0] / SHORTEST_FREQS[int(whichSound)]));

			if (urd(dre) > 0.0f)
				fixedGain[i] = walkModesData[i][1] / 1.3;
			else
				fixedGain[i] = walkModesData[i][1] * 1.3;
			modesEnv[i].trigger = 1.0f;
		}
		break;
	case WhichSound::PLASTIC:
		modesNumber = MODES_NUMS[int(whichSound)];
		sourceSound.load("/Game/plastic.wav");
		originEnv.setAttack(1.0f);
		originEnv.setDecay(1);
		originEnv.setSustain(1);
		originEnv.setRelease(100.0f);
		originEnv.trigger = 1.0f;

		for (int i = 0; i < modesNumber; ++i)
		{
			modesEnv[i].setAttack(1.0f);
			modesEnv[i].setDecay(1.0f);
			modesEnv[i].setSustain(1.0f);
			modesEnv[i].setRelease(BASE_RELEASES[int(whichSound)] / (plasticModesData[i][0] / SHORTEST_FREQS[int(whichSound)]));

			if (urd(dre) > 0.0f)
				fixedGain[i] = plasticModesData[i][1] / 2;
			else
				fixedGain[i] = plasticModesData[i][1] * 2;
			modesEnv[i].trigger = 1.0f;
		}
		break;
	default:
		break;
	}
}
