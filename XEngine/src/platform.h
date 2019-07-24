#pragma once
#include "opengl_defines.h"
#include <iostream>
#include <windows.h>
#include <sstream> 

enum LOG_TYPE
{
	VERBOSE = 0,
	LOG = 1,
	WARNING = 2,
	ERR = 3,
};

class Logger
{
public:
	static void Log(const char* log, LOG_TYPE type = LOG)
	{
		int color = 7;
		switch (type)
		{
			case VERBOSE: 
				color = 7;
			break;
			case LOG:
				color = 7;
			break;
			case WARNING:
				color = 6;
			break;
			case ERR: 
				color = 4;
			break;
		}
		
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		std::cout << log << std::endl;
		SetConsoleTextAttribute(hConsole, 7);
	}

	static void Log(const char* log, int n, LOG_TYPE type = LOG)
	{
		std::ostringstream ss;
		ss << log << " " << n;
		Log(ss.str().c_str(), type);
	}

	static void Log(const char* log, unsigned int n, LOG_TYPE type = LOG)
	{
		std::ostringstream ss;
		ss << log << " " << n;
		Log(ss.str().c_str(), type);
	}

	static void Log(const char* log, float n, LOG_TYPE type = LOG)
	{
		std::ostringstream ss;
		ss << log << " " << n;
		Log(ss.str().c_str(), type);
	}

	static void Log(std::string& log, LOG_TYPE type = LOG)
	{
		Log(log.c_str(), type);
	}
};

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