#include "Level1.h"

#define LEVEL1_WIDTH 9
#define LEVEL1_HEIGHT 18

#define LEVEL1_ENEMY_COUNT 1
unsigned int level1_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 0, 3,
 3, 0, 0, 0, 0, 0, 0, 4, 3,
 3, 0, 0, 0, 1, 1, 1, 2, 3,
 3, 0, 1, 1, 2, 2, 2, 0, 3, 
 3, 0, 0, 0, 0, 0, 0, 0, 3, 
 3, 1, 0, 0, 0, 0, 0, 0, 3, 
 3, 2, 1, 0, 0, 0, 0, 0, 3, 
 3, 2, 2, 1, 0, 0, 0, 0, 3, 
 3, 2, 2, 2, 1, 0, 0, 0, 3, 
 3, 0, 0, 0, 0, 0, 0, 1, 3, 
 3, 0, 0, 0, 0, 0, 1, 1, 3, 
 3, 0, 0, 0, 0, 1, 2, 2, 3, 
 3, 1, 1, 1, 1, 2, 2, 2, 3, 
 3, 2, 2, 2, 2, 2, 2, 2, 3, 
};

GLuint fontID;
void Level1::Initialize() {

    state.nextScene = -1;
    fontID = Util::LoadTexture("font2.png");
	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 5, 1);
	// Move over all of the player and enemy code from initialization.

     // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -17, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -6.81f, 0);
    state.player->speed = 2.40f;
    state.player->textureID = Util::LoadTexture("player.png");

    state.player->animRight = new int[11]{11,12,13,14,15,16,17,18,19,20,21};
    state.player->animLeft = new int[11]{0,1,2,3,4,5,6,7,8,9,10 };
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
    state.player->jumpPower = 4.80f;


    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("ctg.png");
    state.enemies->entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4, -2.25, 0);
    state.enemies->speed = 1;
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].isActive = false;

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
        if (lives > 0) state.nextScene = 1;   // reset the player
    }

    if (lives < 0) {
        lives = 0;
    }

    if (state.player->position.y >= -4.4 && state.player->position.x >= 6.5) {
        state.nextScene = 2;
    }

    return lives;
}




void Level1::Render(ShaderProgram* program, int lives) {
	state.map->Render(program);
	state.player->Render(program);

    Util::DrawText(program, fontID, "Jump! ", 0.27, -0.07f, glm::vec3(6.5, -4, 0));

    if (lives > 0) {
        Util::DrawText(program, fontID, "lives: " + std::to_string(lives), 0.4, -0.05f, glm::vec3(1, -17, 0));
    }

    if (lives == 0) {
        
        Util::DrawText(program, fontID, "You Lose", 0.4, -0.05f, glm::vec3(state.player->position.x, state.player->position.y, 0));
    }
}
