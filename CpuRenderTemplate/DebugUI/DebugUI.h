#pragma once

#include "SDL.h"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_sdl2.h"
#include "Imgui/imgui_impl_opengl3.h"

class Time;

class DebugUI
{
public:
	DebugUI(SDL_Window* window, SDL_GLContext openglContext, const Time& time);

	void ProcessEvents(SDL_Event& event);

	void RenderUI();

	void DrawTitleBar();

	~DebugUI();
private:
	const Time& _time;
	SDL_Window* _window;
};
