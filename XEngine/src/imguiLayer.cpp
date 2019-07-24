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
	if (!ImGui::Begin("Animation Window", &b, windowFlags))
	{}
	float f = 0.0f;
	ImGui::SliderFloat("playback speed", &f, -1.0f, 1.0f, "%.2f", 1.0f);
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