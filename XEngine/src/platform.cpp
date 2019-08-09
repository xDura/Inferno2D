#include "platform.h"

void* Memory::gameMemory = NULL;

const Uint8* Input::state = NULL;
Uint8 Input::prevState[SDL_NUM_SCANCODES];

int Input::mouseState;
int Input::prevMouseState;
int Input::mouseX = 0;
int Input::mouseY = 0;
int Input::prevMouseX = 0;
int Input::prevMouseY = 0;
int Input::mouse_dx = 0;
int Input::mouse_dy = 0;

SDL_Window* Input::window = NULL;

void Input::Init(SDL_Window* a_window)
{
	window = a_window;
	state = SDL_GetKeyboardState(NULL);
	mouseX = mouseY = prevMouseX = prevMouseY = 0;
}

void Input::Update()
{
	prevMouseX = mouseX;
	prevMouseY = mouseY;

	//read keyboard state and store it in keystate
	memcpy((void*)&prevState, state, SDL_NUM_SCANCODES);
	prevMouseState = mouseState;

	SDL_PumpEvents();
	state = SDL_GetKeyboardState(NULL);
	mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	mouse_dx = mouseX - prevMouseX;
	mouse_dy = mouseY - prevMouseY;
}

std::string Path::GetBasePath()
{
	return SDL_GetBasePath();
}

std::string Path::GetPath(const std::string& relativePath)
{
	return SDL_GetBasePath() + relativePath;
}

void Input::CenterMouse()
{
	int window_width, window_height;
	SDL_GetWindowSize(window, &window_width, &window_height);

	int center_x = (int)floor(window_width*0.5f);
	int center_y = (int)floor(window_height*0.5f);
	SDL_WarpMouseInWindow(window, center_x, center_y); //put the mouse back in the middle of the screen
	Input::mouseX = center_x;
	Input::mouseY = center_y;
}

double Time::GetTimeInSeconds()
{
	return (float)SDL_GetTicks() / (float)1000;
}