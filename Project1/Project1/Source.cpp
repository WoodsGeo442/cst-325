#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Source.h"

GLFWwindow* initialize_glfw() {
	// Initialize the context
	if (!glfwInit()) {
		std::cout << "glfwInit(...) failed\n";
		abort();
	}

	// Ask for a core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		std::cout << "glfwCreateWindow(...) failed\n";
		glfwTerminate();
		abort();
	}

	// Make the context current
	glfwMakeContextCurrent(window);

	// Load the function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	return window;
}


void compile_shader() {} // TODO: implement later
void load_geometry() {} // TODO: implement later
void render_scene() {
	// Put the clear and swap code in here
}
void cleanup(GLFWwindow* window) {
	// Call glfw terminate here
}

int main(void) {
	GLFWwindow* window = initialize_glfw();
	compile_shader();
	load_geometry();

	while (!glfwWindowShouldClose(window)) {
		render_scene(window);
		glfwPollEvents();
	}

	cleanup();
	return 0;
}