#include "LEVEL2.h"
#define LEVEL2_WIDTH 14
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 1
unsigned int level2_data[] =
{
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
 3, 1, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3, 3, 3,
 3, 2, 2, 2, 2, 2, 2, 2, 0, 3, 3, 3, 3, 3
};


void Level2::Initialize() {
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tiles.png");
	state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 4, 1);
	// Move over all of the player and enemy code from initialization.

     // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, 0, 0);
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
    state.player->width = 0.6f;
    state.player->jumpPower = 4.50f;


    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("enemy.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(13, -3, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, -0.81, 0);
    state.enemies[0].aiType = WALKER;
    state.enemies[0].aiState = IDLE;

}
void Level2::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; ++i) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.map);
        if (state.player->playerAttack(&state.enemies[i]) == true) {
            state.enemies[i].isActive = false;   // enemy dies 
        }
        if (state.player->enemyCollide(&state.enemies[i]) == true) {
            state.player->isActive = false;   // player dies 
        }


        if (state.player->position.x >= 12) {
            state.nextScene = 2;
        }
    }
}
void Level2::Render(ShaderProgram* program) {
	state.map->Render(program);
	state.player->Render(program);
    state.enemies->Render(program);
}
