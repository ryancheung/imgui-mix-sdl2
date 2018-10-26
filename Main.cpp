#include "SDL.h"
#include "SDL_mixer.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <GL/gl3w.h>

#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

const char* SetupImGui()
{
    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    return glsl_version;
}

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

int main(int, char**)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return 1;
    }

    gWindow = SDL_CreateWindow( "ImGui Mix with SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    if(gWindow == NULL)
    {
        printf( "Window could not be created! %s\n", SDL_GetError() );
        return 1;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL)
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return 1;
    }

    ///---Start ImGUI---///

    const char* glsl_version = SetupImGui();

    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    SDL_Window* window = gWindow;
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gl3wInit() != 0;

    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsClassic();

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool show_demo_window = true;

    int startTicks = SDL_GetTicks();
    //Start counting frames per second
    int countedFrames = 0;

    while (true)
    {
        //FPS cap start tick
        int fpsCapStartTicks = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                return 0;

            ImGui_ImplSDL2_ProcessEvent(&event);
        }

        float avgFPS = countedFrames / ((SDL_GetTicks() - startTicks) / 1000.f);
        if( avgFPS > 2000000 )
            avgFPS = 0;

        // Start Draw graphics with SDL
        SDL_SetRenderDrawColor(gRenderer, clear_color.x * 255, clear_color.y * 255, clear_color.z * 255, clear_color.w * 255);
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, 0xEE, 0xEE, 0xEE, 0xFF);

        static SDL_Point points[4] = {
            {320, 200},
            {300, 240},
            {340, 240},
            {320, 200}
        };

        SDL_RenderDrawLines(gRenderer, points, 4);

        static SDL_Point points2[4] = {
            {320-80, 200},
            {300-80, 240},
            {340-80, 240},
            {320-80, 200}
        };

        SDL_RenderDrawLines(gRenderer, points2, 4);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Rendering
        ImGui::Render();
        //SDL_GL_MakeCurrent(window, gl_context); !!!This line must be commented
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w); !!!This line must be commented
        //glClear(GL_COLOR_BUFFER_BIT); !!!This line must be commented
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        SDL_RenderPresent(gRenderer);

        ++countedFrames;

        // If frame finished early
        int frameTicks = SDL_GetTicks() - fpsCapStartTicks;
        if( frameTicks < SCREEN_TICK_PER_FRAME )
        {
            //Wait remaining time
            SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
