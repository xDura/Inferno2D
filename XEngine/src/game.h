#pragma once
#include "opengl_defines.h"

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
};