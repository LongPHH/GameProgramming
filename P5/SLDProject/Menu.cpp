#include "Menu.h"

GLuint fontID2;
glm::vec3 vec = glm::vec3(-4.25, 3, 0);

void Menu::Initialize() {
	state.nextScene = -1;
	fontID2 = Util::LoadTexture("font2.png");

}

void Menu::Update(float deltatime) {

}

void Menu::Render(ShaderProgram* program ) {
	Util::DrawText(program, fontID2, "Bootleg Mario!", 0.8f, -0.5f, vec);
}