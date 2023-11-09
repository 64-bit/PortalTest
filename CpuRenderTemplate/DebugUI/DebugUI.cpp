#include "stdafx.h"
#include "DebugUI.h"
#include "Timer.h"
#include "RayProfiler.h"
#include "SDL.h"

DebugUI::DebugUI(SDL_Window* window, SDL_GLContext openglContext, const Time& time)
	: _time(time)
{
	_window = window;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, openglContext);
	ImGui_ImplOpenGL3_Init();

}

void DebugUI::ProcessEvents(SDL_Event& event)
{
	// (Where your code calls SDL_PollEvent())
	ImGui_ImplSDL2_ProcessEvent(&event); // Forward your event to backend


}

void DebugUI::RenderUI()
{
	// (After event loop)
// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow(); // Show demo window! :)

	DrawTitleBar();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	RayProfiler.ResetFrame();
}

ImVec2 dragStartPos = ImVec2(0, 0);

void DebugUI::DrawTitleBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		bool hov = ImGui::IsMouseDragging(0);
		if (hov)
		{
			printf("");
		}

		// Check if title bar is being held.
		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
		{
			if (ImGui::IsMouseDragging(0))
			{
				ImVec2 dragDelta = ImGui::GetMouseDragDelta(0);


				int currentX, currentY;

				SDL_GetWindowPosition(_window, &currentX, &currentY);

				SDL_SetWindowPosition(_window, currentX + dragDelta.x, currentY + dragDelta.y);

				// dragDelta contains the delta x,y values by which the title bar was dragged.

				// Store the start position on the first frame of dragging
				if (dragDelta.x == 0 && dragDelta.y == 0)
				{
					dragStartPos = ImGui::GetMousePos();
				}
			}
		}
		else
		{
			// Reset drag start position when mouse is released
			dragStartPos = ImVec2(0, 0);
		}

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("LOL NOTHING HERE", "CTRL+F")) {} //Pay respects
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
	


		char text[128];
		float time = _time.DeltaTime * 1000.0f;

		float rays = RayProfiler.GetRayCount();
		float raysPerSecond = rays / _time.DeltaTime;
		float megaRays = raysPerSecond / 1000000.0f;

		sprintf_s(text, 128, "--- PORTAL BASTARD 9000 ---  mS: %f -- MEGA RAYS/S: %f", time, megaRays);

		ImGui::Text(text);
		ImGui::EndMainMenuBar();
	}
}

DebugUI::~DebugUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
