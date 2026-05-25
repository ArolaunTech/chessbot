#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#ifndef RENDERER_H
#define RENDERER_H

struct RenderSettings {

};

class Renderer {
private:
	GLFWwindow* window;
public:
	Renderer(RenderSettings settings);
	~Renderer();

	void render();
	bool shouldclose() const;
};

#endif