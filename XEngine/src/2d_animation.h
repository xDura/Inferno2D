#pragma once 
#include "utils.h"
#include <vector>

enum SPRITE_ANIMATION_FLAGS
{
	LOOP = 1 << 1,
};

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
	int flags;
};

class SpriteAnimator2
{
public:
	SpriteAnimation* currentAnimation;
	float normalizedTime;
	int currentFrameIndex;

	void Update(float delta);
};