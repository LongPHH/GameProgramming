#include "LEVEL3.h"
#define LEVEL3_WIDTH 14
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 1
unsigned int level3_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2,
 3, 0, 0, 0, 0, 0, 1, 1, 0, 3, 3, 3, 3, 3,
 3, 1, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3,
 3, 2, 2, 2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3
};

glm::vec3 randomvec3 = glm::vec3(1, -1, 0);
glm::vec3 random3 = glm::vec3(10, -3, 0);
GLuint fontID3;

void Level3::Initialize() {
    state.nextScene = -1;
    fontID3 = Util::LoadTexture("font2.png");
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    // Move over all of the player and enemy code from initialization.

     // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, 0, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -6.81f, 0);
    state.player->speed = 2.40f;
    state.player->textureID = Util::LoadTexture("player.png");

    state.player->animRight = new int[3]{ 0,1,2 };
    state.player->animLeft = new int[3]{ 3,4,5 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 6;
    state.player->animRows = 1;


    state.player->height = 0.8f;
    state.player->width = 0.6f;
    state.player->jumpPower = 4.50f;


    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(11, -3, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, -5.81, 0);
    state.enemies[0].aiType = FLOATER;
    state.enemies[0].aiState = FLOATING;
    

}
int Level3::Update(float deltaTime, int lives) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    
    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    if (state.player->playerAttack(&state.enemies[0]) == true) {
        state.enemies[0].isActive = false;   // enemy dies 
    }
    if (state.player->enemyCollide(&state.enemies[0]) == true) {
        state.player->isActive = false;   // player dies 
    }

    if (state.player->isActive == false) {
        lives -= 1;
        if (lives > 0) {
            state.nextScene = 1;   // reset the player
        }
    }
    if (state.player->position.x >= 12.9) {
        state.player->speed = 0;
        state.player->jumpPower = 0;
    }
    
    // fall out of map
    if (state.player->position.y <= -7) {
        state.player->isActive = false;
    }

    return lives;
        
}
void Level3::Render(ShaderProgram* program,int lives) {
    state.map->Render(program);
    state.player->Render(program);
    state.enemies->Render(program);
    if (lives > 0) {
        Util::DrawText(program, fontID3, "lives: " + std::to_string(lives), 0.4, -0.05f, randomvec3);
    }

    if (lives == 0) {
        Util::DrawText(program, fontID3, "You Lose", 0.4, -0.05f, glm::vec3(state.player->position.x, -3, 0));

    }

    if ( lives > 0 && state.player->position.x >= 12.9) {
        Util::DrawText(program, fontID3, "You Win", 0.8, -0.05f, random3);
    }
}

