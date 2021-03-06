#pragma once
#include "game.h"
#include "math.h"
#include "mathTests.h"
#include "camera.h"
#include "imguiLayer.h"
#include "mesh_loaders.h"
#include "pool.h"
#include "ECS/entity_manager.h"
#include "ECS/system_manager.h"
#include "asset_manager.h"
#include "SDL2_Mixer/SDL_mixer.h"

Mesh debugLines;
Camera* camera;
Texture* tex;
Shader* lineShader;
Shader* tiledShader;
Shader* simpleShader;
ImguiLayer* imguiLayer;

Mat44f model;
Mesh quadMesh;

//static defs
Texture* Game::tileTex;
float Game::timeScale = 1.0f;
bool Game::needsShutDown = false;
int Game::tileSizeX = 24;
int Game::tileSizeY = 1;
int Game::tileIndex = 1;
SDL_Window* Game::window = NULL;
SDL_GLContext* Game::glContext = NULL;
SpriteAnimation Game::idleAnimation;
SpriteAnimator2 Game::animator;

SpriteAnimation walkAnimation;
bool invertX = false;
bool lookingRight = true;

std::vector<int> ints;

SpriteSheet* environtmentSpriteSheet;
EntityManager entityManager;

Mix_Music *music = NULL;
float Game::musicVolume = 2.0f;

Tilemap* t2;
std::vector<Entity*> tilemapEntites;

Level* level;

void Game::Init(SDL_Window* a_window, SDL_GLContext* a_context)
{
	window = a_window;
	glContext = a_context;
	imguiLayer = new ImguiLayer();
	imguiLayer->Init(window, glContext);
}

void Game::StartUp()
{
	Input::Init(window);
	/*unsigned int memsize = (unsigned int)10000;
	void* m = Memory::AllocateGameMemory(memsize);*/

	simpleShader = AssetManager::GetShader("data/Shaders/simpleVert.vs", "data/Shaders/simpleFrag.ps");
	tiledShader = AssetManager::GetShader("data/Shaders/tileVert.vs", "data/Shaders/tileFrag.ps");
	tex = AssetManager::GetTexture("data/Sprites/DinoSprites_mort.png");
	tileTex = AssetManager::GetTexture("data/Sprites/tiles.png");
	environtmentSpriteSheet = AssetManager::GetSpriteSheet("data/SpriteSheets/environtmentSpriteSheet_1.xml");

	//TODO: move this audio stuff
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) LOGERROR("Error initializing mixer");

	// Load our music
	music = Mix_LoadMUS(Path::GetPath("data/Audio/background_music1.ogg").c_str());
	if (music == NULL) LOGERROR("Error loading music");

	if (Mix_PlayMusic(music, -1) == -1) LOGERROR("Error playing music");
	Mix_VolumeMusic(musicVolume);
	//***

	entityManager.InitPools();
	t2 = AssetManager::GetTilemap("data/Tilemaps/tileMap_1.xml");

	level = AssetManager::GetLevel("data/Levels/level01.xml");
	TilemapToEntities();

	animator.currentAnimation = &idleAnimation;
	animator.normalizedTime = 0.0f;
	animator.currentFrameIndex = 0;

	idleAnimation.startIndex = 0;
	idleAnimation.endIndex = 3;
	idleAnimation.totalFrames = 4;
	idleAnimation.flags = SPRITE_ANIMATION_FLAGS::LOOP;
	idleAnimation.framesPerSecond = 1.2f;
	idleAnimation.totalSeconds = 5 * (1.0f / 1.2f);

	walkAnimation.startIndex = 4;
	walkAnimation.endIndex = 9;
	walkAnimation.totalFrames = 6;
	walkAnimation.flags = SPRITE_ANIMATION_FLAGS::LOOP;
	walkAnimation.framesPerSecond = 1.3f;
	walkAnimation.totalSeconds = 6 * (1.0f / 1.3f);

	//we want it to be edgy
	glDisable(GL_MULTISAMPLE);

	quadMesh = Mesh();
	Vec3f size = Vec3f(1.0f, 1.0f, 1.0f);
	generateQuad(&quadMesh);

	model = Mat44f();
	model.translateLocal(0.0f, 3.6f, 0.0f);

	Vec3f camPos = Vec3f(10.0f, 10.0f, 40.0f);
	Vec3f camTarget = Vec3f(10.0f, 10.0f, 0.0f);
	Vec3f camUp = Vec3f(0.0f, 1.0f, 0.0f);

	int width, height;
	SDL_GetWindowSize(window, &width, &height);
	//camera = new Camera(camPos, camTarget, camUp, -10.0f, 10.0f, -7.0f, 7.0f, 0.1f, 10000.f);
	camera = new Camera(camPos, camTarget, camUp, 60.0f, width / (float)height, 0.1f, 10000.f);
}

void Game::Update(float deltaTime)
{
	deltaTime *= timeScale;
	Input::Update();

	////******** MOVE THIS TO ONE OTHER PLACE : UPDATEINPUT **************
	Vec3f cameraDir;
	if (Input::state[SDL_SCANCODE_W])
		cameraDir.z = 1.0f;
	else if (Input::state[SDL_SCANCODE_S])
		cameraDir.z = -1.0f;

	if (Input::state[SDL_SCANCODE_D])
		cameraDir.x = -1.0f;
	else if (Input::state[SDL_SCANCODE_A])
		cameraDir.x = 1.0f;

	if (Input::state[SDL_SCANCODE_Q])
		cameraDir.y = 1.0f;
	else if (Input::state[SDL_SCANCODE_E])
		cameraDir.y = -1.0f;

	cameraDir = cameraDir * (float)deltaTime;
	if (Input::state[SDL_SCANCODE_LSHIFT])
		cameraDir *= 80.f;

	if ((Input::mouseState & SDL_BUTTON_MIDDLE))
	{
		camera->RotateLocal(0, Input::mouse_dx * (float)deltaTime * 0.01f, 0.0f);
		camera->RotateLocal(Input::mouse_dy * (float)deltaTime * 0.01f, camera->getLocalVector(Vec3f(1.0f, 0.0f, 0.0f)));
		Input::CenterMouse();
	}


	if (Input::state[SDL_SCANCODE_LEFT])
	{
		lookingRight = false;
		Vec3f translation = Vec3f(-5.0f, 0.0f, 0.0f);
		translation = translation * deltaTime;
		model.translateLocal(translation);
		if (animator.currentAnimation != &walkAnimation)
			animator.normalizedTime = 0.0f;
		animator.currentAnimation = &walkAnimation;
	}
	else if (Input::state[SDL_SCANCODE_RIGHT])
	{
		lookingRight = true;
		Vec3f translation = Vec3f(5.0f, 0.0f, 0.0f);
		translation = translation * deltaTime;
		model.translateLocal(translation);
		if (animator.currentAnimation != &walkAnimation)
			animator.normalizedTime = 0.0f;
		animator.currentAnimation = &walkAnimation;
	}
	else
	{
		if (animator.currentAnimation != &idleAnimation)
			animator.normalizedTime = 0.0f;
		animator.currentAnimation = &idleAnimation;
	}

	camera->MoveLocal(cameraDir);
	camera->UpdateViewProjection();

	//************************************************************

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_0)
				{}
				else if (event.key.keysym.scancode == SDL_SCANCODE_1)
				{}
				if (event.key.keysym.scancode == SDL_SCANCODE_KP_PLUS)
				{}
				else if (event.key.keysym.scancode == SDL_SCANCODE_KP_MINUS)
				{}
				break;
			case SDL_WINDOWEVENT: 
				switch (event.window.event)
				{
					case SDL_WINDOWEVENT_RESIZED:
						glViewport(0, 0, event.window.data1, event.window.data2);
						camera->SetPerspective(60.0f, (float)event.window.data1 / (float)event.window.data2, 0.1f, 10000.f);
					break;
				}
				break;
			case SDL_QUIT:
				Game::needsShutDown = true;
				break;
		}
		imguiLayer->OnEvent(event);
	}

	Mix_VolumeMusic(musicVolume);

	imguiLayer->OnPreRender();
	animator.currentAnimation->totalFrames = animator.currentAnimation->endIndex - animator.currentAnimation->startIndex;
	animator.currentAnimation->totalSeconds = animator.currentAnimation->totalFrames * (1.0f / animator.currentAnimation->framesPerSecond);
	animator.Update(deltaTime);

	//******** MOVE THIS TO ONE OTHER PLACE : RENDERER**************
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SpriteRendererSystem::RenderSprites(&entityManager, tiledShader, camera, &quadMesh);

	Vec2f tileSize;
	tiledShader->enable();
	tiledShader->SetTexture(tex->tex_id);
	tiledShader->SetMat44("M", model * camera->viewProjectionMat);
	tileSize = Vec2f((float)tileSizeX, (float)tileSizeY);
	tiledShader->SetVector2("tileSize", tileSize);
	tiledShader->SetInt("tileIndex", animator.currentFrameIndex);
	tiledShader->SetBool("invertX", !lookingRight);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(quadMesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, quadMesh.length);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	tiledShader->disable();

	imguiLayer->OnPostRender();

	SDL_GL_SwapWindow(window);

	//********************************************************
}

void Game::ReloadTileMap() 
{
	//TODO: coment on TilemapToEntites
	for (unsigned int i = 0; i < tilemapEntites.size(); i++) entityManager.DestroyEntity(tilemapEntites[i]);

	tilemapEntites.clear();
	AssetManager::ReloadTilemaps();
	AssetManager::ReloadLevels();
	t2 = AssetManager::GetTilemap("data/Tilemaps/tileMap_1.xml");
	level = AssetManager::GetLevel("data/Levels/level01.xml");
	TilemapToEntities();
}

void Game::TilemapToEntities() 
{
	//TODO not only remove this
	//but also each tile should not be an entity
	//instead add a TilemapRenderer component
	//and a Tilemap collider 
	for (unsigned int i = 0; i < level->tilemaps.size(); i++)
	{
		Tilemap* t = level->tilemaps[i];
		RENDERER_LAYERS layer = (RENDERER_LAYERS)level->tilemap_layers[i];

		COMPONENT_ID transformAndSprite = component_id(COMPONENT_ID::SPRITE_RENDERER | COMPONENT_ID::TRANSFORM);
		for (int i = 0; i < t->height; i++)
		{
			for (int j = 0; j < t->width; j++)
			{
				unsigned int desiredIndex = (i * t->width) + j;
				int spriteValue = t->tileValues[desiredIndex];
				if (spriteValue != -1)
				{
					Entity* entity = entityManager.CreateEntity(transformAndSprite);
					tilemapEntites.push_back(entity);
					Transform* trans = (Transform*)entity->GetComponent(COMPONENT_ID::TRANSFORM);
					SpriteRenderer* r = (SpriteRenderer*)entity->GetComponent(COMPONENT_ID::SPRITE_RENDERER);
					r->layer = layer;

					int currentTilemapY = t->height - i;
					int currentTilemapX = j;

					trans->transform.setIdentity();
					trans->transform.translate(currentTilemapX * t->tileWidth * 2, currentTilemapY * t->tileHeight * 2, (float)layer);
					trans->transform.scale(Vec3f((float)t->tileWidth, (float)t->tileHeight, 0.0f));
					r->spriteIndex = t->tileValues[desiredIndex];
					r->spriteSheet = t->spriteSheet;
				}
			}
		}
	}

}

void Game::FixedUpdate(float deltaTime)
{
}

void Game::ShutDown()
{
	//TODO: move this audio stuff
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	//**

	//TODO: entityManager.Destroy()
	imguiLayer->ShutDown();
	AssetManager::Destroy();
	free(imguiLayer);
}
