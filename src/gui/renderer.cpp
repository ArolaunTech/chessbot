#include <stdexcept>
#include <iostream>

#include "renderer.h"

void error_callback(int code, const char* description) {
	std::cerr << code << "\n";
	std::cerr << description << "\n";
}

Renderer::Renderer(RenderSettings settings) {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		throw std::runtime_error("GLFW failed to initialize!");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "chessbot", NULL, NULL);

	if (!window) {
		throw std::runtime_error("Window failed to initialize!");
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
}

Renderer::~Renderer() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::render() {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Renderer::shouldclose() const {
	return glfwWindowShouldClose(window);
}