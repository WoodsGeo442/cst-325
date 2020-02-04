#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* initialize_glfw() {
	// ...start with the same init code as before...
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