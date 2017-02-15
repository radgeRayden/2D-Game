#include <iostream>
#include <cstdint>

#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cpplog/cpplog.hpp>

cpplog::StdErrLogger logger;

float GetDeltaTime() {
    static uint64_t lastMeasure = 0;
    uint64_t currentMeasure = SDL_GetPerformanceCounter();
    uint64_t delta = currentMeasure - lastMeasure;
    lastMeasure = currentMeasure;

    return (delta) / (float)SDL_GetPerformanceFrequency();
}

void GameUpdate(float dt) {

}

void GameDraw() {

}


int main(int argc, char** argv) {
    /* Setup window and OpenGL context */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);
    //TODO: make resolution configurable
    //TODO: Add SDL error handling / logging
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    gl3wInit();
    glEnable(GL_MULTISAMPLE);

    glm::vec4 clearColor(0.1f, 0.1f, 0.1f, 1.0f);

    /* Main loop */
    bool done = false;
    SDL_Event event;
    float deltaTime = GetDeltaTime(); //needs to be called once before the loop so we discard the initialization time.
    while (!done) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    //TODO: it's suggested to handle the QUIT event, or add code after the main loop to handle the exit.
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    break;
                case SDL_KEYUP:
                    break;
                default:
                    break;
            }
        }

        GameUpdate(deltaTime);

        /* Basic rendering maintenance */
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        GameDraw();
        
        SDL_GL_SwapWindow(window);

        //let the processor catch a breath
        SDL_Delay(1);
    }

    /* Clean up */
    SDL_GL_DeleteContext(glcontext);
    // I normally destroy the window before anything else for maximum responsiveness but that might be undesirable.
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}