#include "2d_animation.h"
#include <math.h>

void SpriteAnimator::Update(float delta)
{
	float normalizedTimeToAdvance = delta * currentAnimation->framesPerSecond;
	normalizedTime += normalizedTimeToAdvance;

	if(currentAnimation->loop)
		normalizedTime = fmod((normalizedTime), 1.0f);
	else
		normalizedTime = clamp(normalizedTime, 0.0f, 1.0f);

	int frameNumber = normalizedTime * currentAnimation->totalSeconds;
	currentFrameIndex = currentAnimation->startIndex + frameNumber;
	currentFrameIndex = clamp(currentFrameIndex, currentAnimation->startIndex, currentAnimation->endIndex);
}
