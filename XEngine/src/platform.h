#pragma once
#include "opengl_defines.h"
#include <iostream>
#include <windows.h>
#include <sstream> 
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "debug.h"

class Path
{
public:
	static std::string GetBasePath();
	static std::string GetPath(const std::string& relativePath);
};

class Time
{
public:
	static double GetTimeInSeconds();
};

class Input
{
public:
	static int mouseX;
	static int mouseY;
	static int prevMouseX;
	static int prevMouseY;
	static int mouse_dx;
	static int mouse_dy;
	static const Uint8* state;
	static Uint8 prevState[];
	static int mouseState;
	static int prevMouseState;
	static SDL_Window* window;

	static void Init(SDL_Window* a_window);
	static void Update();
	static void CenterMouse();
};