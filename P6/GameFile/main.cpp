#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <SDL_mixer.h>

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Effects.h"
#include "iostream"



SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

// Add some variables and SwitchToScene function 
Scene* currentScene;
Scene* sceneList[5];
Effects* effects;

int playerLives;
Mix_Music* music;
Mix_Chunk* jump;


void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Desktop!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 515, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("music.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 5);

    jump = Mix_LoadWAV("jump.wav");
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/effects_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new Level4();
    SwitchToScene(sceneList[0]);

    playerLives = 4;

    effects = new Effects(projectionMatrix, viewMatrix);
    //effects->Start(FADEIN, 3.2f);

}

void ProcessInput() {
    
    if (currentScene != sceneList[0]) {
        currentScene->state.player->movement = glm::vec3(0);
    }
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                        
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                        
                    case SDLK_SPACE:
                        if (currentScene != sceneList[0]) {
                            if (currentScene->state.player->collidedBottom) {
                                currentScene->state.player->jump = true;
                                Mix_PlayChannel(-1, jump, 0);
                            }
                        }
                        break;
                    case SDLK_RETURN:
                        if (currentScene == sceneList[0]) {
                            SwitchToScene(sceneList[1]);
                        }
                        break;
                }
                break; // SDL_KEYDOWN
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (currentScene != sceneList[0]) {
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animLeft;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
            currentScene->state.player->animIndices = currentScene->state.player->animRight;
        }
        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
bool lastCollidedBottom = false;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
       
        playerLives =  currentScene->Update(FIXED_TIMESTEP, playerLives);

        if (currentScene==sceneList[4] && lastCollidedBottom==false && currentScene->state.enemies[0].collidedBottom) {
            effects->Start(SHAKE, 2.0f);
            
        }
        if(currentScene == sceneList[4])
        {
            lastCollidedBottom = currentScene->state.enemies[0].collidedBottom;
        }
            
        effects->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
    viewMatrix = glm::mat4(1.0f);
    
    if (currentScene != sceneList[0]) {
        if (currentScene->state.player->position.y > -13.75) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, -currentScene->state.player->position.y, 0));
        }
        else {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 13.75f, 0));
        }
    }
    viewMatrix = glm::translate(viewMatrix, effects->viewOffset);

}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);
    currentScene->Render(&program, playerLives);
    //std::cout << "lives " << playerLives;
    effects->Render();

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0) {
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }

        Render();
    }
    
    Shutdown();
    return 0;
}
