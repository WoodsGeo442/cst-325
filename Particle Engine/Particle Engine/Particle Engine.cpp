// Particle Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

using namespace std;

GLuint tex;

struct randomthing {
	float x;
	float y;

	float r;
	float g;
	float b;
	float a;

	float xspeed = 0.5;
	float yspeed = 0.5;
	float yacc= -0.5;

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

	return window;
}


GLuint compile_shader() {
	// Define shader sourcecode
	const char* vertex_shader_src =
		"#version 330 core\n"
		"layout (location = 1) in vec2 texcoords;\n"
		"layout (location = 0) in vec3 pos;\n"
		"uniform vec2 offset;\n"
		"out vec2 Texcoords;\n"
		"void main() {\n"
		"   Texcoords = texcoords;\n"
		"   vec2 scale = vec2(0.5, 0.5);\n"
		"   gl_Position = vec4(scale.x * pos.x + offset.x, scale.y * pos.y + offset.y, pos.z, 1.0);\n"
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
			0.0f, 1.0f, 0.0f,   0.0, 1.0,// top center
			0.0f, 0.0f, 0.0f,  0.0, 0.0,// bottom right
			1.0f, 0.0f, 0.0f,  1.0, 0.0,// bottom left

			0.0f, 1.0f, 0.0f,   0.0, 1.0,
			1.0f, 1.0f, 0.0f,  1.0, 1.0,
			1.0f, 0.0f, 0.0f,  1.0, 0.0,
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
	unsigned char* p = stbi_load("eggplant.png", &x, &y, &n, 0);
	//float pixels[] = {
	//	0.0f, 0.0f, 0.0f,	1.0f, 1.0f, 1.0f, // r, g, b,   r, g, b
	//	1.0f, 1.0f, 1.0f,	0.0f, 0.0f, 0.0f, // r, g, b,   r, g, b
	//};

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
	GLuint tex_location = glGetUniformLocation(shader_program, "tex");
	glUniform1i(tex_location, 0);

	return tex;
}

void render_scene(GLFWwindow* window, GLsizei vertex_count, GLuint shader_program, vector<randomthing> particles) {
	// Set the clear color
	glClearColor(0.7f, 0.0f, 0.5f, 1.0f);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
	
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
	//glBlendFunc(GL_ONE, GL_ONE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	for (int i = 0; i < 100; i++) {
		particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, false));
	}

	load_geometry(&vao, &vbo, &vertex_count);

	while (!glfwWindowShouldClose(window)) {
		time = glfwGetTime();
		float dt = time - oldtime;
		//cout << dt << endl;
		
		for (int i = 0; i < particles.size(); i++) {
			particles[i].yspeed += particles[i].yacc * dt;
			particles[i].x += particles[i].xspeed * dt;
			particles[i].y += particles[i].yspeed * dt;
			particles[i].timer -= dt;

			/*if (particles[i].isTrail == false) {
				particles.push_back(randomthing(particles[i].x, particles[i].y, true));
			}
			if (particles[i].isTrail == true) {
				particles[i].a -= 0.5;
			}*/

			//delete from array while itterating
			if (particles[i].x > 1.0 ) {
				particles[i].xspeed = 0.9 * -abs(particles[i].xspeed);
				//particles[i].a -= 0.5;
				//particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
				
			} 
			if (particles[i].x < -1.0) {
				particles[i].xspeed = 0.9 * abs(particles[i].xspeed);
				//particles[i].a -= 0.2;
				//particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
			}
			if (particles[i].y > 1.0) {
				particles[i].yspeed = 0.9 * -abs(particles[i].yspeed);
				//particles[i].a -= 0.2;
				//particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
				
			}
			if (particles[i].y < -1.0) {
				particles[i].yspeed = 0.9 * abs(particles[i].yspeed);
				//particles[i].a -= 0.2;
				//particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX));
			}
		}
		// add pushback
		
		render_scene(window, vertex_count, shader_program, particles);
		oldtime = time;
		glfwPollEvents();
	}

	cleanup(window, load_texture(shader_program));
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
