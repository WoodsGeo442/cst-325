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
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>


using namespace std;

GLuint tex;

struct Camera {
	glm::mat4 camera_from_world = glm::mat4(1);

	float fov = 60.0f;
	float near = 0.1f;
	float far = 1000.0f;

	glm::mat4 view_from_camera(GLFWwindow* window) {
		//code to calculate view matrix
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0.0f, 0.0f, width, height);
		return glm::perspective(glm::radians(this->fov), float(width) / float(height), this->near, this->far);

	}

};

struct Model {
	GLuint vao;
	GLuint vbo;
	GLsizei vertex_count;

	static Model load(string temp) {
		Model model;

		vector<GLfloat> vertices;

		// Send the vertex data to the GPU
		GLsizei stride = 8 * sizeof(GLfloat);
		{
			fstream file(temp);
			if (!file) {
				cout << "could not find file\n";
				abort();
			}
			string line;
			vector<glm::vec3> positions;
			vector<glm::vec3> normals;
			vector<glm::vec2> texcoords;
			vector<GLfloat> vertices;
			while (getline(file, line)) {
				istringstream line_stream(line);
				string type;
				line_stream >> type;
				if (type == "v") {
					GLfloat x, y, z;
					line_stream >> x;
					line_stream >> y;
					line_stream >> z;
					positions.push_back(glm::vec3(x, y, z));

				}
				else if (type == "vn") {
					GLfloat x, y, z;
					line_stream >> x;
					line_stream >> y;
					line_stream >> z;
					normals.push_back(glm::vec3(x, y, z));

				}
				else if (type == "vt") {
					GLfloat u, v;
					line_stream >> u;
					line_stream >> v;
					texcoords.push_back(glm::vec2(u, v));

				}
				else if (type == "f") {
					string face;
					for (int i = 0; i < 3; ++i) {
						line_stream >> face;
						replace(face.begin(), face.end(), '/', ' ');
						istringstream face_stream(face);
						size_t position_index;
						size_t texcoord_index;
						size_t normal_index;

						face_stream >> position_index;
						face_stream >> texcoord_index;
						face_stream >> normal_index;

						glm::vec3 position = positions.at(position_index - 1); // 1 based indices
						glm::vec3 normal = normals.at(normal_index - 1); // 1 based indices
						glm::vec2 texcoord = texcoords.at(texcoord_index - 1); // 1 based indices

						vertices.push_back(position.x);
						vertices.push_back(position.y);
						vertices.push_back(position.z);
						vertices.push_back(texcoord.x);
						vertices.push_back(texcoord.y);
						vertices.push_back(normal.x);
						vertices.push_back(normal.y);
						vertices.push_back(normal.z);

					}

				}

			}


			model.vertex_count = vertices.size();

			// Use OpenGL to store it on the GPU
			{
				// Create a Vertex Buffer Object on the GPU
				glGenBuffers(1, &model.vbo);
				// Tell OpenGL that we want to set this as the current buffer
				glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
				// Copy all our data to the current buffer!
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			}
		}

		// Tell the GPU how to interpret our existing vertex data
		{

			// Create a Vertex Array Object to hold the settings
			glGenVertexArrays(1, &model.vao);

			// Tell OpenGL that we want to set this as the current vertex array
			glBindVertexArray(model.vao);

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

			glVertexAttribPointer(
				2, // index
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized
				stride, // stride (how far to the next repetition)
				(void*)(5 * sizeof(GLfloat)) // first component
				);

			// Enable the 0th vertex attrib array!
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		}
		return model;
	}

	void cleanup()
	{
		glDeleteVertexArrays(1, &this->vao);
		glDeleteBuffers(1, &this->vbo);
	}
	void draw()
	{
		glBindVertexArray(this->vao);
		glDrawArrays(GL_TRIANGLES, 0, this->vertex_count);
	}

};

struct randomthing {
	glm::mat4 world_from_model = glm::mat4(1.0); // init to the identity matrix
	float x;
	float y;

	float r;
	float g;
	float b;
	float a;

	float xspeed = 0.5;
	float yspeed = 0.5;
	float zspeed = 0.5;

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
		"layout (location = 0) in vec3 pos;\n"
		"layout (location = 1) in vec2 texcoords;\n"
		"layout(location = 2) in vec3 normal;\n"
		"uniform vec2 offset;\n"
		"uniform mat4 camera_from_world;\n"
		"uniform mat4 view_from_camera;\n"
		"uniform mat4 world_from_model;\n"
		"out vec2 Texcoords;\n"
		"out vec3 Normal;\n"
		"out vec3 world_space_position;\n"
		"out vec3 world_space_camera_position;\n"
		"void main() {\n"
		"   Texcoords = texcoords;\n"
		/*"   Normal = normal;\n"*/
		"   Normal = mat3(transpose(inverse(world_from_model))) * normal;\n"
		"   vec2 scale = vec2(0.2, 0.2);\n"
		"   world_space_position = vec3(world_from_model * vec4(pos, 1.0));\n"
		"   mat4 world_from_camera = inverse(camera_from_world);\n"
		"   world_space_camera_position = vec3(world_from_camera * vec4(0.0, 0.0, 0.0, 1.0));\n"


		//To get camera to work
		"   gl_Position = camera_from_world * vec4(pos.xyz, 1.0);\n"

		//For 3D Modeler
		"   gl_Position =  view_from_camera * camera_from_world * vec4(pos.xyz, 1.0);\n"

		//use if offsetting
		"   gl_Position = view_from_camera * camera_from_world * world_from_model * vec4(pos, 1.0);\n"

		"}\n";
	const char* fragment_shader_src =
		"#version 330 core\n"
		"in vec2 Texcoords;\n"
		"in vec3 Normal;\n"
		"in vec3 world_space_position;\n"
		"in vec3 world_space_camera_position;\n"
		"uniform vec4 color;\n"
		"uniform sampler2D tex;\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"   vec3 normal = normalize(Normal);\n"

		//lighting settings
		"   vec3 light_dir = normalize(vec3(1.0, 1.0, 1.0));\n"
		"   vec3 light_color = vec3(0.7,0.5,0.5);\n "
		"   vec3 specular_color = 0.2 * vec3(1.0,1.0,1.0);\n"


		"   vec3 ambient = vec3(0.1, 0.1, 0.1);\n"
		"   float diffuse_intensity = max(dot(normal, light_dir), 0.0);\n"
		"   vec3 diffuse = light_color * diffuse_intensity;\n"

		//Specular lighting
		"   vec3 view_dir = normalize(world_space_camera_position - world_space_position);\n"
		"   vec3 reflect_dir = reflect(-light_dir, normal);\n"
		"   float specular_intensity = pow(max(dot(view_dir, reflect_dir), 0.0), 16);\n"
		"   vec3 specular = specular_intensity * specular_color;\n"

		"   FragColor = vec4(0.5 * (Normal + vec3(1.0)), 1.0);\n"
		"   FragColor = vec4(ambient, 1.0);\n"
		"   FragColor = vec4(0.5 * (world_space_position + vec3(1.0)), 1.0);\n"
		"   FragColor = vec4(ambient + diffuse, 1.0);\n"

		//Using specullar lighting
		"   FragColor = vec4(ambient + diffuse + specular, 1.0);\n"

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

	//using image
	/*int x, y, n;
	unsigned char* p = stbi_load("fire.png", &x, &y, &n, 0);*/

	float pixels[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
	};


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, /*use GL_UNSIGNED_BYTE for images*/GL_FLOAT, pixels);
	GLuint tex_location = glGetUniformLocation(shader_program, "tex");
	glUniform1i(tex_location, 0);

	return tex;
}

void render_scene(GLFWwindow* window, Model model, GLuint shader_program, vector<randomthing> particles, Camera camera) {
	// Set the clear color
	glClearColor(0.7f, 0.0f, 0.5f, 1.0f);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	for (int i = 0; i < particles.size(); i++) {

		//set color
		GLint color_location = glGetUniformLocation(shader_program, "color");
		glUniform4f(color_location, 0.31f, 0.31f, 0.31f, 1.0f);

		//position
		GLint world_from_model = glGetUniformLocation(shader_program, "world_from_model");
		glUniformMatrix4fv(world_from_model, 1, GL_FALSE, glm::value_ptr(particles[i].world_from_model));

		// Draw the current vao/vbo, with the current shader
		glDrawArrays(GL_TRIANGLES, 0, model.vertex_count);

		//view from camera
		GLint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
		glUniformMatrix4fv(view_from_camera_location, 1, GL_FALSE, glm::value_ptr(camera.view_from_camera(window)));

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
glm::mat4 world_from_model = glm::mat4(1.0); // init to the identity matrix



int main(void) {
	vector<randomthing> particles;
	GLFWwindow* window = initialize_glfw();
	GLuint shader_program = compile_shader();
	GLuint texture = load_texture(shader_program);
	Model model;
	Camera camera;
	float time = 0;
	float oldtime = 0;

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	

	camera_from_world = glm::translate(camera_from_world, glm::vec3(0.0f, -1.5f, -8.0f));



	for (int i = 0; i < 1; i++) {
		particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, false));
	}

	model = Model::load("monkey-normals.obj");


	while (!glfwWindowShouldClose(window)) {
		time = glfwGetTime();
		float dt = time - oldtime;

		for (int i = 0; i < particles.size(); i++) {
			particles[i].timer -= dt;

			//move model itself
			particles[i].world_from_model = glm::translate(particles[i].world_from_model, glm::vec3(
				particles[i].xspeed * dt * 0,
				particles[i].yspeed * dt * 0,
				particles[i].zspeed * dt * 0
				));

			//rotate model itself
			particles[i].world_from_model = glm::rotate(particles[i].world_from_model, 0.0001f, glm::vec3(
				particles[i].xspeed * dt * 0,
				particles[i].yspeed * dt,
				particles[i].zspeed * dt * 0
				));

			//Move camera
			/*camera_from_world = glm::translate(camera_from_world, glm::vec3(-0.00005f, 0.0f, 0.0f));*/


			/*camera_from_world = glm::translate(camera_from_world, glm::vec3(0.0f, 0.0f, -0.0001f));*/

			//rotate camera
			/*camera_from_world = glm::rotate(camera_from_world, 0.0001f, glm::vec3(0.0f, 0.0001f, 0.0f));*/



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
		GLint world_from_model_location = glGetUniformLocation(shader_program, "world_from_model");
		glUniformMatrix4fv(
			world_from_model_location,
			1, // count
			GL_FALSE, // transpose
			glm::value_ptr(world_from_model)
			);



		render_scene(window, model, shader_program, particles, camera);
		oldtime = time;
		glfwPollEvents();
	}

	cleanup(window, load_texture(shader_program));
	return 0;
}