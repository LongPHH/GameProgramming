#include "Level4.h"


#define LEVEL4_WIDTH 9
#define LEVEL4_HEIGHT 18

#define LEVEL4_ENEMY_COUNT 1
unsigned int level4_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 4, 4, 4, 4, 4, 4, 4, 3,
 3, 2, 2, 2, 2, 2, 2, 2, 3,
};

GLuint fontID4;
void Level4::Initialize() {
    state.nextScene = -1;
    fontID4 = Util::LoadTexture("font2.png");
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL4_WIDTH, LEVEL4_HEIGHT, level4_data, mapTextureID, 1.0f, 5, 1);
    // Move over all of the player and enemy code from initialization.

     // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -17, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -6.81f, 0);
    state.player->speed = 2.20f;
    state.player->textureID = Util::LoadTexture("player.png");

    state.player->animRight = new int[11]{ 11,12,13,14,15,16,17,18,19,20,21 };
    state.player->animLeft = new int[11]{ 0,1,2,3,4,5,6,7,8,9,10 };
    /*state.player->animUp = new int[8]{ 0,1,2,3,4,5,6,7 };
    state.player->animDown = new int[8]{ 8,9,10,11,12,13,14,15};*/

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 11;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 22;
    state.player->animRows = 1;


    state.player->height = 0.8f;
    state.player->width = 0.6f;
    state.player->jumpPower = 4.8f;


    state.enemies = new Entity[LEVEL4_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");

    state.enemies->entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(3, -16, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].aiType = BOSS;
  


}
int Level4::Update(float deltaTime, int lives) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL4_ENEMY_COUNT, state.map);

    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL4_ENEMY_COUNT, state.map);
    if (state.player->playerAttack(&state.enemies[0]) == true) {
        state.enemies[0].isActive = false;   // enemy dies 
        state.player->isActive = false;
    }
    if (state.player->enemyCollide(&state.enemies[0]) == true) {
        state.player->isActive = false;
    }

    // if die then minus a life and reset
    if (state.player->isActive == false) {
        lives -= 1;
        if (lives > 0) state.nextScene = 4;   // reset the player
        
    }

    if (lives < 0) {
        lives = 0;
    }


    return lives;
}
void Level4::Render(ShaderProgram* program, int lives) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies[0].Render(program);

    

    if (lives > 0) {
        Util::DrawText(program, fontID4, "lives: " + std::to_string(lives), 0.4, -0.05f, glm::vec3(1, -17, 0));
    }

    if (lives == 0) {
        Util::DrawText(program, fontID4, "You Lose", 0.4, -0.05f, glm::vec3(state.player->position.x, state.player->position.y, 0));
    }

    if (lives > 0 && state.enemies[0].isActive == false) {
        Util::DrawText(program, fontID4, "you win", 0.8, -0.05f, glm::vec3(state.player->position.x, state.player->position.y + 2, 0));
    }

}
