#pragma once

#include "maximilian.h"
#include "cstring"

const unsigned short MAX_MODES = 150;

UENUM()
enum class WhichSound : uint8
{
	STEEL,
	WALK,
	PLASTIC
};

class SynthesizedSound
{
public:
	SynthesizedSound();
	virtual ~SynthesizedSound() = default;
	float play();
	void initSoundData(WhichSound whichSound);
	bool getIsPlaying() { return isPlaying; }
private:
	maxiOsc originOsc[MAX_MODES];
	maxiOsc fixedOsc[MAX_MODES];
	maxiEnv modesEnv[MAX_MODES];
	maxiEnv originEnv;
	maxiSample sourceSound;
	bool isPlaying = false;
	bool isPlayOnce = false;
	int tick = 0;
	int modesNumber;

	float fixedGain[MAX_MODES];

	static float steelModesData[MAX_MODES][2];
	static float walkModesData[MAX_MODES][2];
	static float plasticModesData[MAX_MODES][2];

	static float SHORTEST_FREQS[3];
	static float BASE_RELEASES[3];
	static float MODES_NUMS[3];

	static bool isInitOnce;
};

