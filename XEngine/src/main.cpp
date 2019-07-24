#include "main.h"
#include "game.h"
#include "opengl_defines.h"
#include "platform.h"

int defaultWindowWidth = 1000;
int defaultWindowHeight = 800;

int main(int argc, char* argv[])
{
	SDL_Window *window;

	//set attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); //16 or 24
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//antialiasing (disable this lines if it goes too slow)
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetSwapInterval(0);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisample); //increase to have smoother polygons

	SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
										   // Create an application window with the following settings:
	window = SDL_CreateWindow(
		"XDEngine",						   // window title
		SDL_WINDOWPOS_UNDEFINED,           // initial x position
		SDL_WINDOWPOS_UNDEFINED,           // initial y position
		defaultWindowWidth,				   // width, in pixels
		defaultWindowHeight,				// height, in pixels
		SDL_WINDOW_OPENGL                  // flags - see below
	);

	// check for window creation error
	if (window == NULL)
	{
		printf("Error creating window: %s\n", SDL_GetError());
		return 1;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

#ifdef USE_GLEW
	GLenum err = glewInit();
	Logger::Log("Glew init returned: " + err);
#endif
	
	Game::Init(window, &glcontext);
	Game::StartUp();

	bool continueGameLoop = true;
	double lastTime = Time::GetTimeInSeconds();
	while (continueGameLoop) 
	{
		double currentTime = Time::GetTimeInSeconds();
		double elapsed = currentTime - lastTime;
		lastTime = currentTime;

		Game::Update((float)elapsed);
		if (Game::needsShutDown) continueGameLoop = false;
	}

	Game::ShutDown();
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}