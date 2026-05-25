#include <iostream>

#include "types/board.h"
#include "gui/renderer.h"

int main() {
	Board board;

	RenderSettings settings;
	Renderer renderer(settings);

	while (!renderer.shouldclose()) {
		renderer.render();
	}
}