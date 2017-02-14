#include <SDL2/SDL.h>
#include <GL/gl3w.h>

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
    SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    gl3wInit();
    glEnable(GL_MULTISAMPLE);

    /* Main loop */
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                //TODO: it's suggested to handle the QUIT event, or add code after the main loop to handle the exit.
                done = true;
            }
        }

        /* Basic rendering maintenance */
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //TODO: parametrize Clear Color
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(window);

        SDL_Delay(1);
    }

    /* Clean up */
    SDL_GL_DeleteContext(glcontext);
    // I normally destroy the window before anything else for maximum responsiveness but that might be undesirable.
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}