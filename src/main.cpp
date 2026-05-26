#include <iostream>

#include "types/board.h"
#include "gui/renderer.h"

int main() {
	Board board;

	RenderSettings settings;

	settings.bgcolor = Vector4{0.09, 0.08, 0.07, 0};
	settings.lightcolor = Vector4{0.94, 0.84, 0.71, 0};
	settings.darkcolor = Vector4{0.71, 0.53, 0.39, 0};

	Renderer renderer(settings);
	renderer.compileshader("src/gui/shaders/basic.vert", "src/gui/shaders/basic.frag");
	renderer.initbuffersbasic();

	while (!renderer.shouldclose()) {
		renderer.render();
		renderer.endframe();
	}
}