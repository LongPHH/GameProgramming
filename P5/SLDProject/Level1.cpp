#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 1
unsigned int level1_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2,
 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

glm::vec3 randomvec1 = glm::vec3(1, -1, 0);
glm::vec3 random1 = glm::vec3(1, -3, 0);


GLuint fontID;
void Level1::Initialize() {
    state.nextScene = -1;
    fontID = Util::LoadTexture("font2.png");
	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.

     // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -6.81f, 0);
    state.player->speed = 2.40f;
    state.player->textureID = Util::LoadTexture("player.png");
    

    state.player->animRight = new int[3]{0,1,2};
    state.player->animLeft = new int[3]{3,4,5};
   

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 6;
    state.player->animRows = 1;


    state.player->height = 0.8f;
    state.player->width = 0.4f;
    state.player->jumpPower = 4.50f;


    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(7, -5, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, 0, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
}


int Level1::Update(float deltaTime, int lives) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    
    state.enemies[0].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);
    if (state.player->playerAttack(&state.enemies[0]) == true) {
        state.enemies[0].isActive = false;   // enemy dies 
    }
    if (state.player->enemyCollide(&state.enemies[0]) == true) {
        state.player->isActive = false;
    }


    // if die then minus a life and reset
    if (state.player->isActive == false) {
        lives -= 1;
        if (lives > 0) {
            state.nextScene = 1;   // reset the player
        }
    }
    if (state.player->isActive && state.player->position.x >= 12.9) {
        state.nextScene = 2;
    }
    
        
    // fall out of map
    if (state.player->position.y <= -7) {
        state.player->isActive = false;
    }

    return lives;
}
void Level1::Render(ShaderProgram* program,int lives) {
	state.map->Render(program);
	state.player->Render(program);
    state.enemies[0].Render(program);

    if (lives >0) {
        Util::DrawText(program, fontID,"lives: " + std::to_string(lives) , 0.8, -0.5f, randomvec1);
    }
    
    if (lives == 0) {
        Util::DrawText(program, fontID, "You Lose", 0.8, -0.5f, random1);
    }
    
}
