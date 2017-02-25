#include <iostream>
#include <cstdint>
#include <vector>

#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cpplog/cpplog.hpp>
#include <soloud.h>
#include <soloud_wav.h>
#include <soloud_wavstream.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl_gl3.h>

#include "Shader.h"
#include "Timer.h"
#include "Sprite.h"
#include "InputState.h"
#include "SoundFile.h"

//misc globals
static cpplog::StdErrLogger logger;

//rendering globals
static Shader* currentShader;
static std::vector<Sprite> sprites;
static bool vsync;

//audio globals
static SoLoud::Soloud soloud;
static SoLoud::handle backgroundMusic;
static SoLoud::Wav tomatoJingleSource;

//gameplay globals
static glm::vec2 playerPosition(0, 300);

static void GameUpdate(float dt, const InputState currentInputState, const InputState previousInputState) {    
    glm::vec2 playerDirection(0.0f, 0.0f);
    static float playerSpeed = 100.0f; //px per second

    //music control
    if (currentInputState.GetKey(SDLK_SPACE) && !previousInputState.GetKey(SDLK_SPACE)) {
        bool shouldPause = !soloud.getPause(backgroundMusic);
        soloud.setPause(backgroundMusic, shouldPause);
    }
    if (currentInputState.GetKey(SDLK_j) && !previousInputState.GetKey(SDLK_j)) {
        soloud.play(tomatoJingleSource);
    }

    //player movement
    if (currentInputState.GetKey(SDLK_w)) {
        playerDirection += glm::vec2(0.0f, 1.0f);
    }
    if (currentInputState.GetKey(SDLK_a)) {
        playerDirection += glm::vec2(-1.0f, 0.0f);
    }
    if (currentInputState.GetKey(SDLK_s)) {
        playerDirection += glm::vec2(0.0f, -1.0f);
    }
    if (currentInputState.GetKey(SDLK_d)) {
        playerDirection += glm::vec2(1.0f, 0.0f);
    }

//    playerDirection = glm::vec2(1.0f, 0.0f);

    if (glm::length(playerDirection) > 0.0f) {
        playerDirection = glm::normalize(playerDirection);
    }
    
    playerPosition += playerDirection * playerSpeed * dt;
}

static void GameDraw() {
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(glm::floor(playerPosition.x), glm::floor(playerPosition.y), 0.0f));
    glm::mat4 view;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    
    glm::mat4 transform = projection * view * model;

    GLuint transformLoc = glGetUniformLocation(currentShader->ProgramID(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    for (auto& sprite : sprites) {
        sprite.Draw();
    }

    ImGui::ShowMetricsWindow();
}

static void DisplayFileNotFoundError() {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "Missing file",
        "File is missing. Please reinstall the program.",
        NULL);
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
    auto *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    auto glcontext = SDL_GL_CreateContext(window);

    if (vsync) {
        SDL_GL_SetSwapInterval(1);
    }
    else {
        SDL_GL_SetSwapInterval(0);
    }
    
    gl3wInit();
    glEnable(GL_MULTISAMPLE);

    //the mount dir must be set before any game resources are loaded
    PHYSFS_init(argv[0]);
    PHYSFS_mount("../Data", "", true);

    //sound variables
    soloud.init();
  
    SoundFile bgFile(u8"Audio/confection♥core.ogg");
    if (!bgFile.IsValid()) {
        DisplayFileNotFoundError();
        return 0;
    }
    SoLoud::WavStream backgroundMusicSource;
    {
        auto err = backgroundMusicSource.loadFile(&bgFile);
        backgroundMusicSource.setLooping(true);
        backgroundMusicSource.setSingleInstance(true);
        backgroundMusic = soloud.play(backgroundMusicSource);
        if (err) {
            LOG_ERROR(logger) << soloud.getErrorString(err);
        }
    }

    SoundFile sample(u8"Audio/🍅.ogg");
    if (!sample.IsValid()) {
        DisplayFileNotFoundError();
        return 0;
    }
    {
        auto err = tomatoJingleSource.loadFile(&sample);
        if (err) {
            LOG_ERROR(logger) << soloud.getErrorString(err);
        }
    }

    //rendering default variables
    glm::vec4 clearColor(1.0f, 1.0f, 0.0f, 1.0f);
    Shader defaultShader; //load default shader
    currentShader = &defaultShader; //CHANGE THIS
    Sprite testSprite(u8"Sprites/spikeMan_jump.png");
    sprites.push_back(testSprite);
    
    // Setup ImGui binding
    ImGui_ImplSdlGL3_Init(window);

    /* Main loop */
    Timer gameTimer;
    float deltaTime;
    InputState currentInputState;
    InputState previousInputState;

    bool done = false;
    SDL_Event event;

    while (!done) {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSdlGL3_ProcessEvent(&event);
            switch (event.type) {
                case SDL_QUIT:
                    //TODO: it's suggested to handle the QUIT event, or add code after the main loop to handle the exit.
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    currentInputState.SetKey(event.key.keysym.sym, true);
                    break;
                case SDL_KEYUP:
                    currentInputState.SetKey(event.key.keysym.sym, false);
                    break;
                default:
                    break;
            }
        }

        deltaTime = gameTimer.Step();
        GameUpdate(glm::clamp(deltaTime, 0.0f, 1.0f / 30.0f), currentInputState, previousInputState);
        previousInputState = currentInputState;
        //currentInputState.Clear();

        /* Basic rendering maintenance */
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
        
        ImGui_ImplSdlGL3_NewFrame(window);
        //eeww
        if (currentShader->Success()) {
            currentShader->Use(); //we could probably handle this gracefully
        }
        GameDraw();
        
        ImGui::Render();

        SDL_GL_SwapWindow(window);

        //let the processor catch a breath
        if(!vsync) {
            SDL_Delay(1);
        }
        
    }

    /* Clean up */
    SDL_GL_DeleteContext(glcontext);
    // I normally destroy the window before anything else for maximum responsiveness but that might be undesirable.
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}