#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "../types/vector.h"

#ifndef RENDERER_H
#define RENDERER_H

struct RenderSettings {
	Vector4 bgcolor;
	Vector4 lightcolor;
	Vector4 darkcolor;
};

class Renderer {
private:
	GLFWwindow* window;

	RenderSettings settings;

	int width;
	int height;

	std::vector<unsigned int> shaderprograms;

	void renderboard();

	unsigned int vbobasicpos;
	unsigned int vaobasic;
	unsigned int ratioloc;
	unsigned int transformloc;
	unsigned int colloc;
public:
	Renderer(RenderSettings nsettings);
	~Renderer();

	void compileshader(std::string vertexpath, std::string fragmentpath);
	void initbuffersbasic();

	void render();

	void endframe();

	bool shouldclose() const;

	const GLFWwindow* getwindow() const;
};

#endif