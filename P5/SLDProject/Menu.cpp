#include "Menu.h"

GLuint fontID2;
glm::vec3 vec = glm::vec3(-4.5, 1, 0);
glm::vec3 vec2 = glm::vec3(3.2, -2, 0);

void Menu::Initialize() {
	state.nextScene = -1;
	glClearColor(0, 0, 0, 1.0f);
	fontID2 = Util::LoadTexture("font2.png");
}

void Menu::Update(float deltatime) {
	
}

void Menu::Render(ShaderProgram* program ) {
	Util::DrawText(program, fontID2, "Bootleg Mario!", 0.8f, -0.3f, vec);
	Util::DrawText(program, fontID2, "PRESS ENTER", 0.2f, -0.05f, vec2);

}