#pragma once
#include "opengl_defines.h"
#include "math.h"
#include "2d_animation.h"
#include "texture.h"

class Game 
{
public:
	static void Init(SDL_Window* a_window, SDL_GLContext* a_context);
	static void StartUp();
	static void Update(float deltaTime);
	static void FixedUpdate(float deltaTime);
	static void ShutDown();

	//TODO: remove this
	static void ReloadTileMap();
	static void TilemapToEntities();

	static bool needsShutDown;
	static SDL_Window* window;
	static SDL_GLContext* glContext;

	static float timeScale;
	static int tileSizeX;
	static int tileSizeY;
	static int tileIndex;
	static SpriteAnimation idleAnimation;
	static SpriteAnimator2 animator;
	static float musicVolume;

	static Texture* tileTex;
};