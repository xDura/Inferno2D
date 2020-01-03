#pragma once
#include "utils.h"
#include <vector>
#include <map>

struct BoneKeyFrame
{
public:
	double time = 0.0f;
	Vector3 pos;
	Quaternion rot;
	Vector3 scale;
};


class Animation
{
public:
	const char* name = "";

	//this vector stores keyframes for each bone
	//ex: keyframes[0] is a vector that stores all keyframes for
	//the bone with index 0
	std::vector<std::vector<BoneKeyFrame>> keyframes;

	double samplesPerSecond = 0.0f;
	//number of samples for each bone
	int numSamples = 0;

	//TODO: turn this into flags
	bool loop = true;

	~Animation()
	{
		//TODO: check this destructors or directly remove
		//the maps and replace them by a better data structure
		for (size_t i = 0; i < keyframes.size(); i++) keyframes[i].clear();
		keyframes.clear();
	}
};
