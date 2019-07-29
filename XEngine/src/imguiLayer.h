#pragma once
#include "opengl_defines.h"
#include "External/imgui/imgui.h"
#include "External/imgui/imgui_impl_sdl.h"
#include "External/imgui/imgui_impl_opengl2.h"

class ImguiLayer
{
public:
	void Init(SDL_Window* window, SDL_GLContext* context);
	void OnPreRender();
	void OnEvent(const SDL_Event& evnt);
	void OnPostRender();
	void ShutDown();
	void RenderTilemapMenu();
	SDL_Window* window = NULL;
};