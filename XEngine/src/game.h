#pragma once
#include "opengl_defines.h"
#include "math.h"
#include "2d_animation.h"

class Game 
{
public:
	static void Init(SDL_Window* a_window, SDL_GLContext* a_context);
	static void StartUp();
	static void Update(float deltaTime);
	static void FixedUpdate(float deltaTime);
	static void ShutDown();

	static bool needsShutDown;
	static SDL_Window* window;
	static SDL_GLContext* glContext;

	static float timeScale;
	static int tileSizeX;
	static int tileSizeY;
	static int tileIndex;
	static SpriteAnimation animation;
	static SpriteAnimator2 animator;
};