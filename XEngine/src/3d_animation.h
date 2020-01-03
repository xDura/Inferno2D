#pragma once
#include "utils.h"
#include <vector>
#include <map>

struct BoneKeyFrame
{
public:
	f64 time = 0.0f;
	Vec3f pos;
	Quaternion rot;
	Vec3f scale;
};


class Animation
{
public:
	//this vector stores keyframes for each bone
	//ex: keyframes[0] is a vector that stores all keyframes for
	//the bone with index 0
	std::vector<std::vector<BoneKeyFrame>> keyframes;

	f64 samplesPerSecond = 0.0f;
	//number of samples for each bone
	u32 numSamples = 0;

	//TODO: turn this into flags
	//flags8 flags;
	bool loop = true;
	//TODO: pass this name to an outside structure
	String30 name = {""};

	~Animation()
	{
		//TODO: check this destructors or directly remove
		//the maps and replace them by a better data structure
		for (size_t i = 0; i < keyframes.size(); i++) keyframes[i].clear();
		keyframes.clear();
	}
};
