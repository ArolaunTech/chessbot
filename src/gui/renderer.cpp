#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <format>

#include "renderer.h"

void error_callback(int code, const char* description) {
	std::cerr << code << "\n";
	std::cerr << description << "\n";
}

void window_refresh_callback(GLFWwindow* window) {
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

	if (renderer->getwindow() != window) {
		throw std::runtime_error("Renderer pointer associated with window does not correspond to it!");
	}

	renderer->render();

	glfwSwapBuffers(window);
}

Renderer::Renderer(RenderSettings nsettings) {
	settings = nsettings;

	shaderprograms.clear();

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

	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetWindowUserPointer(window, static_cast<void*>(this));

	glfwMakeContextCurrent(window);
	gladLoadGL();

	glfwSwapInterval(1);
}

Renderer::~Renderer() {
	glDeleteBuffers(1, &vbobasicpos);
	glDeleteVertexArrays(1, &vaobasic);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Renderer::compileshader(std::string vertexpath, std::string fragmentpath) {
	// Read files
	std::ifstream vertexstream(vertexpath);
	if (!vertexstream.is_open()) {
		throw std::runtime_error("Could not open vertex shader file");
	}

	std::ifstream fragmentstream(fragmentpath);
	if (!fragmentstream.is_open()) {
		throw std::runtime_error("Could not open fragment shader file");
	}

	std::stringstream vertexbuffer, fragmentbuffer;
	vertexbuffer << vertexstream.rdbuf();
	fragmentbuffer << fragmentstream.rdbuf();

	std::string vertexsource = vertexbuffer.str(), fragmentsource = fragmentbuffer.str();
	const char * vertexsourcecstr = vertexsource.c_str();
	const char * fragmentsourcecstr = fragmentsource.c_str();

	// Compile
	unsigned int vertexshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexshader, 1, &vertexsourcecstr, NULL);
	glCompileShader(vertexshader);

	int success;
	char log[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glDeleteShader(vertexshader);

		glGetShaderInfoLog(vertexshader, 512, NULL, log);
		throw std::runtime_error(std::format("Vertex shader compilation failed: {}", log));
	}

	unsigned int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &fragmentsourcecstr, NULL);
	glCompileShader(fragmentshader);

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);

		glGetShaderInfoLog(fragmentshader, 512, NULL, log);
		throw std::runtime_error(std::format("Fragment shader compilation failed: {}", log));
	}

	// Link
	unsigned int shaderprogram = glCreateProgram();
	glAttachShader(shaderprogram, vertexshader);
	glAttachShader(shaderprogram, fragmentshader);
	glLinkProgram(shaderprogram);

	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	glGetProgramiv(shaderprogram, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(shaderprogram, 512, NULL, log);
		throw std::runtime_error(std::format("Shader linking failed: {}", log));
	}

	shaderprograms.push_back(shaderprogram);
}

void Renderer::initbuffersbasic() {
	glUseProgram(shaderprograms[0]);

	ratioloc = glGetUniformLocation(shaderprograms[0], "ratio");
	if (ratioloc == -1) {
		throw std::runtime_error("Could not find uniform \"ratio\" in shader.");
	}

	transformloc = glGetUniformLocation(shaderprograms[0], "transform");
	if (transformloc == -1) {
		throw std::runtime_error("Could not find uniform \"transform\" in shader.");
	}

	colloc = glGetUniformLocation(shaderprograms[0], "col");
	if (colloc == -1) {
		throw std::runtime_error("Could not find uniform \"col\" in shader.");
	}

	glGenBuffers(1, &vbobasicpos);

	glGenVertexArrays(1, &vaobasic);
	glBindVertexArray(vaobasic);

	glBindBuffer(GL_ARRAY_BUFFER, vbobasicpos);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBufferData(GL_ARRAY_BUFFER, 6 * 1024 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::render() {
	glfwGetFramebufferSize(window, &width, &height);

	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(settings.bgcolor.x, settings.bgcolor.y, settings.bgcolor.z, 1);

	renderboard();	
}

void Renderer::renderboard() {
	GLfloat square[12] = {
		0, 0,
		0, 1,
		1, 1,
		0, 0,
		1, 0,
		1, 1
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbobasicpos);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(GLfloat), square);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(shaderprograms[0]);
	glBindVertexArray(vaobasic);

	glUniform1f(ratioloc, (GLfloat)width / height);

	std::array<GLfloat, 9> matrixdata;
	for (int i = 0; i < 9; i++) {
		matrixdata[i] = 0;
	}
	matrixdata[0] = 0.2;
	matrixdata[4] = 0.2;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if ((i + j) % 2 == 1) {
				glUniform4f(colloc, settings.darkcolor.x, settings.darkcolor.y, settings.darkcolor.z, 1.0);
			} else {
				glUniform4f(colloc, settings.lightcolor.x, settings.lightcolor.y, settings.lightcolor.z, 1.0);
			}

			matrixdata[6] = -0.8 + 0.2 * j;
			matrixdata[7] = 0.6 - 0.2 * i;

			glUniformMatrix3fv(transformloc, 1, GL_FALSE, matrixdata.data());

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
}

void Renderer::endframe() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Renderer::shouldclose() const {
	return glfwWindowShouldClose(window);
}

const GLFWwindow* Renderer::getwindow() const {
	return window;
}