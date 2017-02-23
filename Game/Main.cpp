#include <iostream>
#include <cstdint>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cpplog/cpplog.hpp>
#include <PhysFS++/physfs.hpp>

#include "Shader.h"
#include "Timer.h"
#include "Sprite.h"

static cpplog::StdErrLogger logger;
static Shader* currentShader;
static std::vector<Sprite> sprites;

void GameUpdate(float dt) {

}

void GameDraw() {
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(250, 250, 0.0f));
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    
    glm::mat4 transform = projection * view * model;

    GLuint transformLoc = glGetUniformLocation(currentShader->ProgramID(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    for (auto& sprite : sprites) {
        sprite.Draw();
    }
}


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        //TODO: change this to a file log since the user probably cannot even see this otherwise.
        LOG_ERROR(logger) << SDL_GetError() << std::endl;
        return 1;
    }

    /* Setup window and OpenGL context */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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

    //the mount dir must be set before any game resources are loaded
    PhysFS::init(argv[0]);
    PhysFS::mount("../Data", "", true);

    //rendering default variables
    glm::vec4 clearColor(1.0f, 0.1f, 0.1f, 1.0f);
    Shader defaultShader; //load default shader
    currentShader = &defaultShader; //CHANGE THIS
    Sprite testSprite("Sprites/spikeMan_jump.png");
    sprites.push_back(testSprite);

    /* Main loop */
    Timer gameTimer;
    float deltaTime;

    bool done = false;
    SDL_Event event;

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

        deltaTime = gameTimer.Step();
        GameUpdate(deltaTime);

        /* Basic rendering maintenance */
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        //eeww
        if (currentShader->Success()) {
            currentShader->Use(); //we could probably handle this gracefully
        }
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