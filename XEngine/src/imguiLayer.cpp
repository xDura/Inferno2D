#pragma once
#include "imguiLayer.h"
#include "game.h"

void ImguiLayer::Init(SDL_Window * a_window, SDL_GLContext * a_context)
{
	IMGUI_CHECKVERSION();
	window = a_window;
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(a_window, a_context);
	ImGui_ImplOpenGL2_Init();
}

void ImguiLayer::RenderAnimationMenu()
{
	int windowFlags = 0;
	bool b = true;
	if (!ImGui::Begin("Tile stuff window", &b, windowFlags))
	{}

	ImGui::SliderInt("TileSizeX", &Game::tileSizeX, 1, 24, "%.2f");
	ImGui::SliderInt("TileSizeY", &Game::tileSizeY, 1, 24, "%.2f");
	ImGui::InputInt("TileIndex", &Game::animator.currentFrameIndex, 1);
	ImGui::InputFloat("normalizedTime", &Game::animator.normalizedTime, 1);
	ImGui::InputFloat("totalSeconds", &Game::animator.currentAnimation->totalSeconds, 1);
	ImGui::InputInt("totalFrames", &Game::animator.currentAnimation->totalFrames, 1);
	ImGui::InputInt("AnimationStart", &Game::animation.startIndex, 1);
	ImGui::InputInt("AnimationEnd", &Game::animation.endIndex, 1);
	ImGui::SliderFloat("FramesPerSec", &Game::animation.framesPerSecond, 0, 10, "%.2f", 1);
	ImGui::End();
}


void ImguiLayer::OnPreRender()
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
	bool b = true;
	RenderAnimationMenu();
	//ImGui::ShowDemoWindow(&b);
}

void ImguiLayer::OnEvent(const SDL_Event& evnt)
{
	ImGui_ImplSDL2_ProcessEvent(&evnt);
}

void ImguiLayer::OnPostRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void ImguiLayer::ShutDown()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}