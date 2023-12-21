#include "Trident.h"

int main() {
	Trident::Trident_GX trident(500, 500, "Hi");
	trident.Init();

	Trident::Object test(trident, Trident::Triangle, glm::vec2(0, 0), glm::vec2(1.0f, 1.0f), glm::vec3(0.5f, 1.0f, 1.0f));
	test.SetPremake(Trident::Square);

	while (!trident.windowShouldClose()) {

		trident.Start(glm::vec3(0.0f, 0.0f, 0.0f));
		trident.useDefulatShader();

		test.Move(trident.getInputDirection() * 0.01f);
		test.Render();

		trident.End();
	}

	trident.Clearup();
	return 0;
}