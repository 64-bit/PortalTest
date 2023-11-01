#include <stdafx.h>

#include <SDL.h>
#undef main

#include <GL/glew.h>
#include <iostream>
#include <Windows.h>

#include "FrameDisplayer.h"
#include "GenTexture.h"
#include "Timer.h"

#include "stdafx.h"
#include "Graphics/Texture2D.h"


const int width = 1920, height = 1080;

bool running = true;

Texture2D textureA(width, height);
Texture2D textureB(width, height);

void HandleSDLEvent(SDL_Event e) 
{
    if (e.type == SDL_QUIT)
    {
        running = false;
    }
}

Texture2D* DrawFrame(float time, int frame, int width, int height)
{
    bool isOdd = frame % 2;
    return isOdd ? &textureA : &textureB;
}

int main(int argc, char** argv)
{  
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("OpenGL", 400, 400, width, height, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(window);
    glewInit();

    SDL_GL_SetSwapInterval(0);

    FrameDisplayer displayer = FrameDisplayer();

    GenerateTexture(textureA, 0);
    GenerateTexture(textureB, 1);

    Time time = Time(true, 100);

    while(running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            HandleSDLEvent(e);
        }

        bool isOdd = time.CurrentFrame % 2;
        Texture2D* texture = isOdd ? &textureA : &textureB;

        //Rendering
        ///////////////////////////////////////////////
        texture = DrawFrame(time.DeltaTime, time.CurrentFrame, width, height);
        displayer.DisplayFrame(texture);

        SDL_GL_SwapWindow(window);
        ///////////////////////////////////////////////

        time.OnFrame();
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}