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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, leftMatrix, rightMatrix, birdieMatrix, projectionMatrix;

glm::vec3 left_movement = glm::vec3(0, 0, 0);
glm::vec3 left_position = glm::vec3(-4.4, 0, 0);

glm::vec3 right_movement = glm::vec3(0, 0, 0);
glm::vec3 right_position = glm::vec3(4.4, 0, 0);

glm::vec3 birdie_movement = glm::vec3(0, 0, 0);



float left_speed = 1.0f;
float right_speed = 1.0f;
float birdie_speed = 1.0f;
float lastTicks = 0.0f;

float start = false;

GLuint racket_left, racket_right, birdie;


GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Badminton!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    leftMatrix = glm::mat4(1.0f);
    rightMatrix = glm::mat4(1.0f);
    birdieMatrix = glm::mat4(1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Good setting for transparency

    racket_left = LoadTexture("racket_left.png");
    racket_right = LoadTexture("racket_right.png");
    birdie = LoadTexture("birdie.png");
}

void ProcessInput() {
    left_movement = glm::vec3(0);
    right_movement = glm::vec3(0);
    birdie_movement = glm::vec3(0);
    

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
                // Move the player up
                break;

            case SDLK_RIGHT:
                // Move the player right
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            }

            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    // HOLDING ON THE A KEY ON KEYBOARD TO MOVE
    if (keys[SDL_SCANCODE_W]) {
        left_movement.y = 2.0f;
    }
    else if (keys[SDL_SCANCODE_S]) {
        left_movement.y = -2.0f;
    }

     if (keys[SDL_SCANCODE_UP]) {
        right_movement.y = 2.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        right_movement.y = -2.0f;
    }

    // MAKE SURE LEFT PLAYER, RIGHT PLATER AND BALL HAVE THE SAME SPEED.
    if (glm::length(left_movement) > 2.0f) {
        left_movement = glm::normalize(left_movement);
    }

    if (glm::length(right_movement) > 2.0f) {
        right_movement = glm::normalize(right_movement);
    }

    if (glm::length(left_movement) > 2.0f) {
        birdie_movement = glm::normalize(left_movement);
    }

    if (keys[SDL_SCANCODE_SPACE]) {
        start = true;
    }
}



void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    // Add (direction * units per second * elapsed time)
    left_position += left_movement * left_speed * deltaTime;
    leftMatrix = glm::mat4(1.0f);
    leftMatrix = glm::translate(leftMatrix, left_position);
    
    right_position += right_movement * right_speed * deltaTime;
    rightMatrix = glm::mat4(1.0f);
    rightMatrix = glm::translate(rightMatrix, right_position);
    
}


void DrawObject(glm::mat4& matrix, GLuint& textureID) {
    program.SetModelMatrix(matrix);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    program.SetModelMatrix(leftMatrix);
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    DrawObject(leftMatrix, racket_left);
    DrawObject(rightMatrix, racket_right);
    DrawObject(birdieMatrix, birdie);
   
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

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
        Render();
    }
    
    Shutdown();
    return 0;
}
