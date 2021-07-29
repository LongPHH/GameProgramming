
#include "Scene.h"
class Level1 : public Scene {

public:
	void Initialize() override;
	int Update(float deltaTime, int lives) override;
	void Render(ShaderProgram* program,int lives) override;
};