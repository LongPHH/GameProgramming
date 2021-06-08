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
glm::mat4 viewMatrix, leviMatrix, projectionMatrix;

glm::mat4 zekeMatrix;

float zeke_movement = 0;
float zeke_direction = -2.0f;
float levi_rotate = 0;
float levi_Xmovement = 0;
float levi_Ymovement = 0;
float levi_Xdirection = 2.0f;
float levi_Ydirection = -2.0f;

GLuint levi_TextureID, zeke_TextureID;

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
    displayWindow = SDL_CreateWindow("Midget vs Monke", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    leviMatrix = glm::mat4(1.0f);
    zekeMatrix = glm::mat4(1.0f);
  

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
   // program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // BLENDING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      // Good setting for transparency

    levi_TextureID = LoadTexture("levi.png");
    zeke_TextureID = LoadTexture("zeke.png");
    
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}


float lastTicks = 0.0f;   // amt of time gone by since stl
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;    // second
    lastTicks = ticks;
    

    if (zeke_movement < -4.3f) {
        zeke_direction = 2.0f;
    }
    else if (zeke_movement > 4.3f) {
        zeke_direction = -2.0f;
    }

    // levi moving left and right
    if (levi_Xmovement < -4.0f) {
        levi_Xdirection = 5.0f;
    }
    else if (levi_Xmovement > 4.0f) {
        levi_Xdirection = -5.0f;
    }

    // Levi moving up and down
    if (levi_Ymovement < -2.5f) {
        levi_Ydirection = 3.0f;
    }
    else if (levi_Ymovement > 1.26f) {
        levi_Ydirection = -3.0f;
    }

    zeke_movement += zeke_direction * deltaTime;
    levi_rotate += 720.0f * deltaTime;

    levi_Xmovement += levi_Xdirection * deltaTime;
    levi_Ymovement += levi_Ydirection * deltaTime;
    
    leviMatrix = glm::mat4(1.0f);
    zekeMatrix = glm::mat4(1.0f);

    // zeke movement
    zekeMatrix = glm::translate(zekeMatrix, glm::vec3(zeke_movement, -2.0f, 0.0f));
    zekeMatrix = glm::scale(zekeMatrix, glm::vec3(2.5f, 2.5f, 2.5f)); // zeke is 17"

    // levi Movement
    
    leviMatrix = glm::translate(leviMatrix, glm::vec3(0.0f, levi_Ymovement, 0.0f));
    leviMatrix = glm::translate(leviMatrix, glm::vec3(levi_Xmovement, 2.50f, 0.0f));
    //leviMatrix = glm::translate(leviMatrix, glm::vec3(levi_Xmovement, 2.50f, 0.0f));

    leviMatrix = glm::rotate(leviMatrix, glm::radians(levi_rotate), glm::vec3(0.0f, 0.0f, 4.0f));    
    leviMatrix = glm::scale(leviMatrix, glm::vec3(-0.5f, -0.5f, -0.5f));    // Levi is 5"2
 
}

void DrawObject(glm::mat4& matrix, GLuint& textureID) {
    program.SetModelMatrix(matrix);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);


    DrawObject(leviMatrix, levi_TextureID);
    DrawObject(zekeMatrix, zeke_TextureID);


   
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
