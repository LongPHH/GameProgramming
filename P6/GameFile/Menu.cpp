#include "Menu.h"

GLuint font2;
glm::vec3 vec = glm::vec3(-4.5, 1, 0);
glm::vec3 vec2 = glm::vec3(-3, -2, 0);

void Menu::Initialize() {
	
	state.nextScene = -1;
	glClearColor(0, 0, 0, 1.0f);
	font2 = Util::LoadTexture("font2.png");
}

int Menu::Update(float deltatime, int lives) {
	return 4; // initialise live will be 4
}

void Menu::Render(ShaderProgram* program, int lives ) {
	Util::DrawText(program, font2, "TOWER OF GODS!", 0.8f, -0.3f, vec);
	Util::DrawText(program, font2, "PRESS ENTER", 0.2f, -0.05f, vec2);

}