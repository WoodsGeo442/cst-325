// Particle Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
// new sdatesafdsf

#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using namespace std;

GLuint tex;

struct Camera {
	glm::mat4 camera_from_world = glm::mat4(1);
	float fov = 60.0f;
	float near = 0.1f;
	float far = 1000.0f;

	glm::mat4 view_from_camera(GLFWwindow* window) {
		//code to calculate view matrix
		glm::mat4 world_from_model = glm::mat4(1.0); // init to the identity matrix
	}
};

struct randomthing {
	float x;
	float y;

	float r;
	float g;
	float b;
	float a;

	float xspeed = 0.5;
	float yspeed = 0.5;
	float yacc = -0.5;

	float timer = 5.0;

	bool isTrail = false;

	randomthing(float X, float Y, bool istrail) {
		x = X;
		y = Y;
		this->isTrail = istrail;
		this->xspeed = 0.5 * (float)rand() / (float)RAND_MAX;
		this->yspeed = 0.5 * (float)rand() / (float)RAND_MAX;
		this->a = 1;
		this->r = 0.4f;
		this->b = 0.1f;
		this->g = 0.0f;
		if (istrail == true) {
			xspeed = 0;
			yspeed = 0;
		}
	}
};

GLFWwindow* initialize_glfw() {
	// Initialize the context
	if (!glfwInit()) {
		cout << "glfwInit(...) failed\n";
		abort();
	}

	// Ask for a core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(640, 480, "Particle Engine - Geoffrey Woods", NULL, NULL);
	if (!window) {
		cout << "glfwCreateWindow(...) failed\n";
		glfwTerminate();
		abort();
	}

	// Make the context current
	glfwMakeContextCurrent(window);

	// Load the function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	//enable 3d shapes
	glEnable(GL_DEPTH_TEST);


	return window;
}


GLuint compile_shader() {
	// Define shader sourcecode
	const char* vertex_shader_src =
		"#version 330 core\n"
		"layout (location = 1) in vec2 texcoords;\n"
		"layout (location = 0) in vec3 pos;\n"
		"uniform vec2 offset;\n"
		"uniform mat4 camera_from_world;\n"
		"uniform mat4 view_from_camera;\n"
		"uniform mat4 world_from_model;\n"
		"out vec2 Texcoords;\n"
		"void main() {\n"
		"   Texcoords = texcoords;\n"
		"   vec2 scale = vec2(0.2, 0.2);\n"

		//To get camera to work
		"   gl_Position = camera_from_world * vec4(pos.xyz, 1.0);\n"

		//For 3D Modeler
		"   gl_Position =  view_from_camera * camera_from_world * vec4(pos.xyz, 1.0);\n"

		//use if offsetting
		/*"   gl_Position = view_from_camera * camera_from_world * world_from_model * vec4(pos, 1.0);\n"*/

		"}\n";
	const char* fragment_shader_src =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 color;\n"
		"uniform sampler2D tex;\n"
		"in vec2 Texcoords;\n"
		"void main() {\n"
		"   vec2 uvs=vec2(gl_FragCoord)/100.0;\n"
		"   FragColor=texture(tex,Texcoords);\n"
		//"   FragColor.a*=a;\n"
		//"   FragColor = color;\n"
		//set alpha 
		"}\n";

	// Define some vars
	const int MAX_ERROR_LEN = 512;
	GLint success;
	GLchar infoLog[MAX_ERROR_LEN];

	// Compile the vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
	glCompileShader(vertex_shader);

	// Check for errors
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex_shader, MAX_ERROR_LEN, NULL, infoLog);
		cout << "vertex shader compilation failed:\n" << infoLog << std::endl;
		abort();
	}

	// Compile the fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
	glCompileShader(fragment_shader);

	// Check for errors
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment_shader, MAX_ERROR_LEN, NULL, infoLog);
		cout << "fragment shader compilation failed:\n" << infoLog << std::endl;
		abort();
	}

	// Link the shaders
	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// Check for errors
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shader_program, MAX_ERROR_LEN, NULL, infoLog);
		cout << "shader linker failed:\n" << infoLog << std::endl;
		abort();
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Enable the shader here since we only have one
	glUseProgram(shader_program);

	return shader_program;
}


void load_geometry(GLuint* vao, GLuint* vbo, GLsizei* vertex_count) {
	// Send the vertex data to the GPU
	GLsizei stride = 5 * sizeof(GLfloat);
	{
		// Generate the data on the CPU
		GLfloat vertices[] = {
			0.0f,  0.0f, 0.0f, 	10.0, 10.0,
			0.0f,  1.0f, 0.0f, 	10.0, 0.0,
			1.0f,  1.0f, 0.0f, 	0.0, 0.0,
			0.0f,  0.0f, 0.0f, 	10.0, 10.0,
			1.0f,  0.0f, 0.0f, 	10.0, 0.0,
			1.0f,  1.0f, 0.0f, 	0.0, 0.0,

			0.0f,  0.0f, 1.0f, 	10.0, 10.0,
			0.0f,  1.0f, 1.0f, 	10.0, 0.0,
			1.0f,  1.0f, 1.0f, 	0.0, 0.0,
			0.0f,  0.0f, 1.0f, 	10.0, 10.0,
			1.0f,  0.0f, 1.0f, 	10.0, 0.0,
			1.0f,  1.0f, 1.0f, 	0.0, 0.0,

			1.0f, 0.0f,  0.0f, 	10.0, 10.0,
			1.0f, 0.0f,  1.0f, 	10.0, 0.0,
			1.0f, 1.0f,  1.0f, 	0.0, 0.0,
			1.0f, 0.0f,  0.0f, 	10.0, 10.0,
			1.0f, 1.0f,  0.0f, 	10.0, 0.0,
			1.0f, 1.0f,  1.0f, 	0.0, 0.0,

			0.0f, 0.0f,  0.0f, 	10.0, 10.0,
			0.0f, 0.0f,  1.0f, 	10.0, 0.0,
			0.0f, 1.0f,  1.0f, 	0.0, 0.0,
			0.0f, 0.0f,  0.0f, 	10.0, 10.0,
			0.0f, 1.0f,  0.0f, 	10.0, 0.0,
			0.0f, 1.0f,  1.0f, 	0.0, 0.0,

			0.0f, 0.0f, 0.0f, 	10.0, 10.0,
			0.0f, 0.0f, 1.0f, 	10.0, 0.0,
			1.0f, 0.0f, 1.0f, 	0.0, 0.0,
			0.0f, 0.0f, 0.0f, 	10.0, 10.0,
			1.0f, 0.0f, 0.0f, 	10.0, 0.0,
			1.0f, 0.0f, 1.0f, 	0.0, 0.0,

			0.0f, 1.0f, 0.0f, 	10.0, 10.0,
			0.0f, 1.0f, 1.0f, 	10.0, 0.0,
			1.0f, 1.0f, 1.0f, 	0.0, 0.0,
			0.0f, 1.0f, 0.0f, 	10.0, 10.0,
			1.0f, 1.0f, 0.0f, 	10.0, 0.0,
			1.0f, 1.0f, 1.0f, 	0.0, 0.0,
		};

		*vertex_count = sizeof(vertices) / sizeof(vertices[0]);

		// Use OpenGL to store it on the GPU
		{
			// Create a Vertex Buffer Object on the GPU
			glGenBuffers(1, vbo);
			// Tell OpenGL that we want to set this as the current buffer
			glBindBuffer(GL_ARRAY_BUFFER, *vbo);
			// Copy all our data to the current buffer!
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		}
	}

	// Tell the GPU how to interpret our existing vertex data
	{

		// Create a Vertex Array Object to hold the settings
		glGenVertexArrays(1, vao);

		// Tell OpenGL that we want to set this as the current vertex array
		glBindVertexArray(*vao);

		// Tell OpenGL the settings for the current 0th vertex array!
		glVertexAttribPointer(
			0, // index
			3, // size
			GL_FLOAT, // type
			GL_FALSE, // normalized
			stride, // stride (how far to the next repetition)
			(void*)0 // first component
			);
		glVertexAttribPointer(
			1, // index
			2, // size
			GL_FLOAT, // type
			GL_FALSE, // normalized
			stride, // stride (how far to the next repetition)
			(void*)(3 * sizeof(GLfloat)) // first component
			);

		// Enable the 0th vertex attrib array!
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
	}
}

GLuint load_texture(GLuint shader_program) {
	glActiveTexture(GL_TEXTURE0);
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	GLsizei width = 2;
	GLsizei height = 2;

	int x, y, n;
	unsigned char* p = stbi_load("fire.png", &x, &y, &n, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
	GLuint tex_location = glGetUniformLocation(shader_program, "tex");
	glUniform1i(tex_location, 0);

	return tex;
}

void render_scene(GLFWwindow* window, GLsizei vertex_count, GLuint shader_program, vector<randomthing> particles) {
	// Set the clear color
	glClearColor(0.7f, 0.0f, 0.5f, 1.0f);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	for (int i = 0; i < particles.size(); i++) {
		
		//set color
		GLint color_location = glGetUniformLocation(shader_program, "color");
		glUniform4f(color_location, 0.0f, 0.0f, 0.0f, 1.0f);

		//position
		GLint offset_location = glGetUniformLocation(shader_program, "offset");
		glUniform2f(offset_location, particles[i].x, particles[i].y);

		// Draw the current vao/vbo, with the current shader
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);


	}


	// Display the results on screen
	glfwSwapBuffers(window);
}

void cleanup(GLFWwindow* window/*, GLuint *shader_program*/, GLuint load_texture/*, GLuint vao, GLuint vbo*/) {
	/*glDeleteProgram(*shader_program);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);*/
	glDeleteTextures(1, &tex);
	glfwTerminate();
}

//Add matrix for 3D
glm::mat4 camera_from_world = glm::mat4(1.0); // init to the identity matrix


int main(void) {
	GLuint vao;
	GLuint vbo;
	GLsizei vertex_count;
	vector<randomthing> particles;
	GLFWwindow* window = initialize_glfw();
	GLuint shader_program = compile_shader();
	GLuint texture = load_texture(shader_program);
	float time = 0;
	float oldtime = 0;

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint world_to_camera_location = glGetUniformLocation(shader_program, "camera_from_world");
	glUniformMatrix4fv(
		world_to_camera_location,
		1, // count
		GL_FALSE, // transpose
		glm::value_ptr(camera_from_world)
	);

	/*camera_from_world = glm::translate(camera_from_world, glm::vec3(0.0f, 0.0f, -20.0f));*/



	for (int i = 0; i < 1; i++) {
		particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, false));
	}

	load_geometry(&vao, &vbo, &vertex_count);

	while (!glfwWindowShouldClose(window)) {
		time = glfwGetTime();
		float dt = time - oldtime;

		for (int i = 0; i < particles.size(); i++) {
			particles[i].timer -= dt;

			//Move camera
			camera_from_world = glm::translate(camera_from_world, glm::vec3(-0.00005f, 0.0f, 0.0f));
			

			camera_from_world = glm::translate(camera_from_world, glm::vec3(0.0f, 0.0f, -0.0001f));

			//rotate camera
			/*world_from_model = glm::rotate(world_from_model, 0.0001f, glm::vec3(1.0f, 1.0f, 0.0f));*/


			
		}

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0.0f, 0.0f, width, height);
		glm::mat4 view_from_camera = glm::perspective(
			glm::radians(60.0f), 
			(float)width / (float)height, 
			0.1f, 
			100.0f
			);
		
		//sends matrix to shader
		GLint world_to_camera_location = glGetUniformLocation(shader_program, "camera_from_world");
		glUniformMatrix4fv(
			world_to_camera_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(camera_from_world)
			);

		GLint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
		glUniformMatrix4fv(
			view_from_camera_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(view_from_camera)
			);

		//use if moving object
		//GLint world_from_model_location = glGetUniformLocation(shader_program, "world_from_model");
		//glUniformMatrix4fv(
		//	world_from_model_location,
		//	1, // count
		//	GL_FALSE, // transpose
		//	glm::value_ptr(world_from_model)
		//	);





		


		render_scene(window, vertex_count, shader_program, particles);
		oldtime = time;
		glfwPollEvents();
	}

	cleanup(window, load_texture(shader_program));
	return 0;
}