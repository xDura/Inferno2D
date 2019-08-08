#pragma once 
#include "utils.h"
#include <vector>

class SpriteAnimation
{
public:
	std::string name;
	std::string tileSetPath;

	int startIndex;
	int endIndex;
	int totalFrames;
	float totalSeconds;
	float framesPerSecond;
	//turn this bool into more flags if needed
	bool loop;
};

class SpriteAnimator2
{
public:
	SpriteAnimation* currentAnimation;
	float normalizedTime;
	int currentFrameIndex;

	void Update(float delta);
};