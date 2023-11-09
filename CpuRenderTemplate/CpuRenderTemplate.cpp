#include <stdafx.h>

#include <SDL.h>
#undef main

#include <GL/glew.h>
#include <iostream>

#define NOMINMAX 
#include <Windows.h>

#include "FrameDisplayer.h"
#include "GenTexture.h"
#include "Timer.h"

#include "stdafx.h"
#include "Graphics/Texture2D.h"
#include "RayRenderer/RayRenderer.h"
#include "MapLoader.h"
#include "DebugUI/DebugUI.h"


const int OTHER_SCALE = 1;

const int width = 320 * OTHER_SCALE, height = 240 * OTHER_SCALE;
const int SCALE = 4 / OTHER_SCALE;



bool running = true;

Texture2D textureA(width, height);

void HandleSDLEvent(SDL_Event e) 
{
    if (e.type == SDL_QUIT)
    {
        running = false;
    }
}

int main(int argc, char** argv)
{  
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("OpenGL", 400, 200, width * SCALE, height * SCALE, SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewInit();

    SDL_GL_SetSwapInterval(0);

    FrameDisplayer displayer = FrameDisplayer();

    Time time = Time(true, 100);

    RayRenderer rayRenderer;
    World world;

    FILE* file;

    auto error = fopen_s(&file, "../Assets/level.map", "rb");
    world.LoadFromFile(file);
    fclose(file);

    error = fopen_s(&file, "../Assets/block01a.jpg", "rb");
    world.Texture = new Texture2D(file);
    fclose(file);


    rayRenderer.BackgroundColor = Color_b(0, 0, 0, 255);

    float pitch = 0.0f;
    float yaw = 0.0f;

    Camera camera;

    const auto newPitch = quat(vec3(pitch, 0.0f, 0.0f));
    const auto newYaw = quat(vec3(0.0f, yaw, 0.0f));

    camera.Rotation = newYaw * newPitch;
    camera.Position = glm::vec3(0.0f, 0.0f, 4.0f);
    camera.fovY = 1.0f;

    DebugUI debugUI = DebugUI(window, context, time);


    while(running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            debugUI.ProcessEvents(e);

            //if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
            {
                HandleSDLEvent(e);
            }
        }

        //Logic Update
        //if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
        {
            camera.Move(world, time.DeltaTime);
        }

        //Rendering
        ///////////////////////////////////////////////
        rayRenderer.RenderWorld(textureA, world, camera);
        displayer.DisplayFrame(&textureA);
        debugUI.RenderUI();

        SDL_GL_SwapWindow(window);
        ///////////////////////////////////////////////

        time.OnFrame();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}