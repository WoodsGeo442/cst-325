// Particle Engine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#define STB_IMAGE_IMPLEMENTATION
//#define STBI_ASSERT(x)
//#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GLFW\glfw3.h>
//#include <iostream>
//#include <vector>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <string>
//#include "glm/glm.hpp"
//#include "glm/gtc/type_ptr.hpp" 
//#include<algorithm>
//
//using namespace std;
//
//GLuint tex;
//
//
//struct Model {
//	GLuint vao;
//	GLuint vbo;
//	GLsizei vertex_count;
//
//	void draw(){
//        glBindVertexArray(this->vao);
//        glDrawArrays(GL_TRIANGLES, 0, this->vertex_count);
//    }
//
//    static Model load(){
//        Model model;
//        vector<GLfloat> vertices;
//        {
//            //load geometry from file
//            {
//                fstream file("teapot.obj");
//                if (!file)
//                {
//                    cout << "could not find model file\n";
//                    abort();
//                }
//                vector<glm::vec3> positions;
//                vector<glm::vec2> texcoords;
//                string line;
//                while (getline(file, line))
//                {
//                    //cout << line << "\n";
//                    istringstream line_stream(line);
//                    string type;
//                    line_stream >> type;
//                    if (type == "v")
//                    {
//                        GLfloat x, y, z;
//                        line_stream >> x;
//                        line_stream >> y;
//                        line_stream >> z;
//                        positions.push_back(glm::vec3(x, y, z));
//                    }
//                    else if (type == "vt")
//                    {
//                        GLfloat u, v;
//                        line_stream >> u;
//                        line_stream >> v;
//                        texcoords.push_back(glm::vec2(u, v));
//                    }
//                    else if (type == "f")
//                    {
//                        string face;
//                        for (int i = 0; i < 3; i++)
//                        {
//                            line_stream >> face;
//                            replace(face.begin(), face.end(), '/', ' ');
//                            size_t position_index;
//                            size_t texcoord_index;
//                            istringstream face_stream(face);
//                            face_stream >> position_index;
//                            face_stream >> texcoord_index;
//                            glm::vec3 position = positions.at(position_index - 1);
//                            glm::vec2 texcoord = texcoords.at(texcoord_index - 1);
//                            vertices.push_back(position.x);
//                            vertices.push_back(position.y);
//                            vertices.push_back(position.z);
//                            vertices.push_back(texcoord.x);
//                            vertices.push_back(texcoord.y);
//                        }
//                    }
//                }
//            }
//            model.vertex_count = vertices.size();
//            {
//                //create vertex buffer object on GPU
//                glGenBuffers(1, &model.vbo);
//                //set to current buffer 
//                glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
//                //copy data to current buffer
//                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
//            }
//        }
//        {
//            //create VAO
//            glGenVertexArrays(1, &model.vao);
//            //set as current vertex array
//            glBindVertexArray(model.vao);
//            GLsizei stride = 5 * sizeof(GLfloat);
//            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
//            glEnableVertexAttribArray(0);
//            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
//            glEnableVertexAttribArray(1);
//        }
//        return model;
//    }
//
//};
//
//struct randomthing {
//	float x;
//	float y;
//	glm::mat4 world_from_model = glm::mat4(1.0);
//
//	float r;
//	float g;
//	float b;
//	float a;
//
//	float xspeed = 0.5;
//	float yspeed = 0.5;
//	float yacc= -0.5;
//
//	float timer = 5.0;
//
//	bool isTrail = false;
//
//	randomthing(float X, float Y, bool istrail) {
//		glm::translate(this->world_from_model, glm::vec3(X, Y, 0.0f));
//		this->isTrail = istrail;
//		this->xspeed = 0.5 * (float)rand() / (float)RAND_MAX;
//		this->yspeed = 0.5 * (float)rand() / (float)RAND_MAX;
//		this->a = 1;
//		this->r = 0.4f;
//		this->b = 0.1f;
//		this->g = 0.0f;
//		if (istrail == true) {
//			xspeed = 0;
//			yspeed = 0;
//		}
//	}
//};
//
//struct Camera {
//	glm::mat4 camera_from_world = glm::mat4(1.0);
//	float fov = 10.0f;
//	float near = 0.1f;
//	float far = 100.0f;
//
//	glm::mat4 view_from_camera(GLFWwindow* window ){
//		int width, height;
//		glfwGetWindowSize(window, &width, &height);
//		glViewport(0.0f, 0.0f, width, height);
//		return glm::perspective(
//			glm::radians(this-> fov),
//			(float)width / (float)height, // aspect ratio
//			this-> near, // near
//			this-> far // far
//			);
//	}
//
//
//};
//
//GLFWwindow* initialize_glfw() {
//	// Initialize the context
//	if (!glfwInit()) {
//		cout << "glfwInit(...) failed\n";
//		abort();
//	}
//
//	// Ask for a core profile
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// Create the window
//	GLFWwindow* window = glfwCreateWindow(640, 480, "Particle Engine - Geoffrey Woods", NULL, NULL);
//	if (!window) {
//		cout << "glfwCreateWindow(...) failed\n";
//		glfwTerminate();
//		abort();
//	}
//
//	// Make the context current
//	glfwMakeContextCurrent(window);
//
//	// Load the function pointers
//	glewExperimental = GL_TRUE;
//	glewInit();
//
//	return window;
//}
//
//GLuint compile_shader() {
//	// Define shader sourcecode
//	const char* vertex_shader_src =
//		"#version 330 core\n"
//		"layout (location = 1) in vec2 texcoords;\n"
//		"layout (location = 0) in vec3 pos;\n"
//		/*"uniform vec2 offset;\n"*/
//		"uniform mat4 camera_from_world;\n"
//		"uniform mat4 view_from_camera;\n"
//		"uniform mat4 world_from_model;\n"
//		"out vec2 Texcoords;\n"
//		"void main() {\n"
//		"   Texcoords = texcoords;\n"
//		"   vec2 scale = vec2(0.2, 0.2);\n"
//		//"   gl_Position = vec4(scale.x * pos.x + offset.x, scale.y * pos.y + offset.y, pos.z, 1.0);\n"
//		//"   gl_Position = camera_from_world * vec4(pos.xyz, 1.0);"
//		"   gl_Position = view_from_camera * camera_from_world * world_from_model * vec4(pos, 1.0);\n"
//
//		"}\n";
//	const char* fragment_shader_src =
//		"#version 330 core\n"
//		"out vec4 FragColor;\n"
//		"uniform vec4 color;\n"
//		"uniform sampler2D tex;\n"
//		"in vec2 Texcoords;\n"
//		"void main() {\n"
//		"   vec2 uvs=vec2(gl_FragCoord)/100.0;\n"
//		"   FragColor=texture(tex,Texcoords);\n"
//		//"   FragColor.a*=a;\n"
//		//"   FragColor = color;\n"
//		//set alpha 
//		"}\n";
//
//	// Define some vars
//	const int MAX_ERROR_LEN = 512;
//	GLint success;
//	GLchar infoLog[MAX_ERROR_LEN];
//
//	// Compile the vertex shader
//	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
//	glCompileShader(vertex_shader);
//
//	// Check for errors
//	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(vertex_shader, MAX_ERROR_LEN, NULL, infoLog);
//		cout << "vertex shader compilation failed:\n" << infoLog << std::endl;
//		abort();
//	}
//
//	// Compile the fragment shader
//	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
//	glCompileShader(fragment_shader);
//
//	// Check for errors
//	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
//	if (!success) {
//		glGetShaderInfoLog(fragment_shader, MAX_ERROR_LEN, NULL, infoLog);
//		cout << "fragment shader compilation failed:\n" << infoLog << std::endl;
//		abort();
//	}
//
//	// Link the shaders
//	GLuint shader_program = glCreateProgram();
//	glAttachShader(shader_program, vertex_shader);
//	glAttachShader(shader_program, fragment_shader);
//	glLinkProgram(shader_program);
//
//	// Check for errors
//	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
//	if (!success) {
//		glGetProgramInfoLog(shader_program, MAX_ERROR_LEN, NULL, infoLog);
//		cout << "shader linker failed:\n" << infoLog << std::endl;
//		abort();
//	}
//	glDeleteShader(vertex_shader);
//	glDeleteShader(fragment_shader);
//
//	// Enable the shader here since we only have one
//	glUseProgram(shader_program);
//
//	return shader_program;
//}
//
//Model load_geometry() {
//	// Send the vertex data to the GPU
//	Model model;
//	GLsizei stride = 5 * sizeof(GLfloat);
//	{
//		// Generate the data on the CPU
//		GLfloat vertices[] = {
//			0.0f,  0.0f, 0.0f, 	10.0, 10.0,
//			0.0f,  1.0f, 0.0f, 	10.0, 0.0,
//			1.0f,  1.0f, 0.0f, 	0.0, 0.0,
//			0.0f,  0.0f, 0.0f, 	10.0, 10.0,
//			1.0f,  0.0f, 0.0f, 	10.0, 0.0,
//			1.0f,  1.0f, 0.0f, 	0.0, 0.0,
//
//			0.0f,  0.0f, 1.0f, 	10.0, 10.0,
//			0.0f,  1.0f, 1.0f, 	10.0, 0.0,
//			1.0f,  1.0f, 1.0f, 	0.0, 0.0,
//			0.0f,  0.0f, 1.0f, 	10.0, 10.0,
//			1.0f,  0.0f, 1.0f, 	10.0, 0.0,
//			1.0f,  1.0f, 1.0f, 	0.0, 0.0,
//
//			1.0f, 0.0f,  0.0f, 	10.0, 10.0,
//			1.0f, 0.0f,  1.0f, 	10.0, 0.0,
//			1.0f, 1.0f,  1.0f, 	0.0, 0.0,
//			1.0f, 0.0f,  0.0f, 	10.0, 10.0,
//			1.0f, 1.0f,  0.0f, 	10.0, 0.0,
//			1.0f, 1.0f,  1.0f, 	0.0, 0.0,
//
//			0.0f, 0.0f,  0.0f, 	10.0, 10.0,
//			0.0f, 0.0f,  1.0f, 	10.0, 0.0,
//			0.0f, 1.0f,  1.0f, 	0.0, 0.0,
//			0.0f, 0.0f,  0.0f, 	10.0, 10.0,
//			0.0f, 1.0f,  0.0f, 	10.0, 0.0,
//			0.0f, 1.0f,  1.0f, 	0.0, 0.0,
//
//			0.0f, 0.0f, 0.0f, 	10.0, 10.0,
//			0.0f, 0.0f, 1.0f, 	10.0, 0.0,
//			1.0f, 0.0f, 1.0f, 	0.0, 0.0,
//			0.0f, 0.0f, 0.0f, 	10.0, 10.0,
//			1.0f, 0.0f, 0.0f, 	10.0, 0.0,
//			1.0f, 0.0f, 1.0f, 	0.0, 0.0,
//
//			0.0f, 1.0f, 0.0f, 	10.0, 10.0,
//			0.0f, 1.0f, 1.0f, 	10.0, 0.0,
//			1.0f, 1.0f, 1.0f, 	0.0, 0.0,
//			0.0f, 1.0f, 0.0f, 	10.0, 10.0,
//			1.0f, 1.0f, 0.0f, 	10.0, 0.0,
//			1.0f, 1.0f, 1.0f, 	0.0, 0.0,
//		};
//
//		model.vertex_count = sizeof(vertices) / sizeof(vertices[0]);
//
//		// Use OpenGL to store it on the GPU
//		{
//			// Create a Vertex Buffer Object on the GPU
//			glGenBuffers(1, &model.vbo);
//			// Tell OpenGL that we want to set this as the current buffer
//			glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
//			// Copy all our data to the current buffer!
//			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//		}
//	}
//
//	// Tell the GPU how to interpret our existing vertex data
//	{
//
//		// Create a Vertex Array Object to hold the settings
//		glGenVertexArrays(1, &model.vao);
//
//		// Tell OpenGL that we want to set this as the current vertex array
//		glBindVertexArray(model.vao);
//
//		GLsizei stride = 5 * sizeof(GLfloat);
//		// Tell OpenGL the settings for the current 0th vertex array!
//		glVertexAttribPointer(
//			0, // index
//			3, // size
//			GL_FLOAT, // type
//			GL_FALSE, // normalized
//			stride, // stride (how far to the next repetition)
//			(void*)0 // first component
//		);
//
//		glVertexAttribPointer(
//			1, // index
//			2, // size
//			GL_FLOAT, // type
//			GL_FALSE, // normalized
//			stride, // stride (how far to the next repetition)
//			(void*)(3 * sizeof(GLfloat)) // first component
//		);
//
//		// Enable the 0th vertex attrib array!
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//	}
//
//	return model;
//}
//
//GLuint load_texture(GLuint shader_program) {
//	glActiveTexture(GL_TEXTURE0);
//	GLuint tex;
//	glGenTextures(1, &tex);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	GLsizei width = 2;
//	GLsizei height = 2;
//
//	int x, y, n;
//	unsigned char* p = stbi_load("fire.png", &x, &y, &n, 0);
//
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, p);
//	GLuint tex_location = glGetUniformLocation(shader_program, "tex");
//	glUniform1i(tex_location, 0);
//
//	return tex;
//}
//
//void render_scene(GLFWwindow* window, Model model, Camera cam, GLuint shader_program, vector<randomthing> &particles) {
//	// Set the clear color
//	glClearColor(0.7f, 0.0f, 0.5f, 1.0f);
//
//	GLint world_to_camera_location = glGetUniformLocation(shader_program, "camera_from_world");
//	glUniformMatrix4fv(
//		world_to_camera_location,
//		1, // count
//		GL_FALSE, // transpose
//		glm::value_ptr(cam.camera_from_world)
//	);
//
//	GLint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
//	glUniformMatrix4fv(
//		view_from_camera_location,
//		1, // count
//		GL_FALSE, // transpose
//		glm::value_ptr(cam.view_from_camera(window))
//	);
//	
//	
//
//
//
//	// Clear the screen
//	glClear(GL_COLOR_BUFFER_BIT);
//	
//	for (int i = 0; i < particles.size(); i++) {
//
//		//set color
//		GLint color_location = glGetUniformLocation(shader_program, "color");
//		glUniform4f(color_location, 0.0f, 0.0f, 0.0f, 1.0f);
//
//		//position
//		/*GLint offset_location = glGetUniformLocation(shader_program, "offset");
//		glUniform2f(offset_location, particles[i].x, particles[i].y);*/
//
//		particles[i].world_from_model = glm::translate(particles[i].world_from_model, glm::vec3(0.0f, 0.0f, 0.10f));
//
//		particles[i].world_from_model = glm::rotate(particles[i].world_from_model, 0.1f, glm::vec3(1.0f, 1.0f, 0.0f));
//
//		GLint world_from_model_location = glGetUniformLocation(shader_program, "world_from_model");
//		glUniformMatrix4fv(
//			world_from_model_location,
//			1,//count
//			GL_FALSE, //transpose
//			glm::value_ptr(particles[i].world_from_model)
//		);
//
//		
//
//		// Draw the current vao/vbo, with the current shader
//		glDrawArrays(GL_TRIANGLES, 0, model.vertex_count);
//
//		
//	}
//	
//
//	// Display the results on screen
//	glfwSwapBuffers(window);
//}
//
//void cleanup(GLuint shader_program, GLuint load_texture, Model model) {
//	glDeleteProgram(shader_program);
//	glDeleteVertexArrays(1, &model.vao);
//	glDeleteBuffers(1, &model.vbo);
//	glDeleteTextures(1, &tex);
//	glfwTerminate();
//}
//
//
//
//int main(void) {
//	vector<randomthing> particles;
//	GLFWwindow* window = initialize_glfw();
//	GLuint shader_program = compile_shader();
//	GLuint texture = load_texture(shader_program);
//    Model model = load_geometry();
//	float time = 0;
//	float oldtime = 0;
//		
//		
//	Camera cam;
//	cam.camera_from_world = glm::translate(cam.camera_from_world, glm::vec3(0.0f, 0.0f, -20.0f));
//
//	glEnable(GL_BLEND);
//
//	// makes the colors brighter and overlaps
//	//glBlendFunc(GL_ONE, GL_ONE);
//
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	
//
//	for (int i = 0; i < 1; i++) {
//		particles.push_back(randomthing((float)rand() / (float)RAND_MAX, (float)rand() / (float)RAND_MAX, false));
//	}
//	
//	
//
//	while (!glfwWindowShouldClose(window)) {
//		time = glfwGetTime();
//		float dt = time - oldtime;
//		//cout << dt << endl;
//		
//		for (int i = 0; i < particles.size(); i++) {
//			/*particles[i].yspeed += particles[i].yacc * dt;*/
//			//particles[i].world_from_model = glm::translate(particles[i].world_from_model, glm::vec3(particles[i].xspeed * dt, particles[i].yspeed * dt, 0.0f));
//			/*particles[i].x += particles[i].xspeed * dt;
//			particles[i].y += particles[i].yspeed * dt;*/
//			particles[i].timer -= dt;
//
//			
//		}
//		//cam.camera_from_world = glm::translate(cam.camera_from_world, glm::vec3(0.001f, 0.0f, 0.001f));
//		render_scene(window, model, cam ,shader_program, particles);
//		oldtime = time;
//		glfwPollEvents();
//	}
//
//	cleanup(load_texture(shader_program), shader_program, model);
//	return 0;
//}



#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp" 
#include <algorithm>
using namespace std;
struct Model
{
    GLuint vao;
    GLuint vbo;
    GLsizei vertex_count;
    static Model load()
    {
        Model model;
        vector<GLfloat> vertices;
        {
            //load geometry from file
            {
                fstream file("teapot.obj");
                if (!file)
                {
                    cout << "could not find model file\n";
                    abort();
                }
                vector<glm::vec3> positions;
                vector<glm::vec3> normals;
                vector<glm::vec2> texcoords;
                string line;
                while (getline(file, line))
                {
                    //cout << line << "\n";
                    istringstream line_stream(line);
                    string type;
                    line_stream >> type;
                    if (type == "v")
                    {
                        GLfloat x, y, z;
                        line_stream >> x;
                        line_stream >> y;
                        line_stream >> z;
                        positions.push_back(glm::vec3(x, y, z));
                    }
                    else if (type == "vn")
                    {
                        GLfloat x, y, z;
                        line_stream >> x;
                        line_stream >> y;
                        line_stream >> z;
                        normals.push_back(glm::vec3(x, y, z));
                    }
                    else if (type == "vt")
                    {
                        GLfloat u, v;
                        line_stream >> u;
                        line_stream >> v;
                        texcoords.push_back(glm::vec2(u, v));
                    }
                    else if (type == "f")
                    {
                        string face;
                        for (int i = 0; i < 3; i++)
                        {
                            line_stream >> face;
                            replace(face.begin(), face.end(), '/', ' ');
                            size_t position_index;
                            size_t texcoord_index;
                            size_t normal_index;

                            istringstream face_stream(face);
                            face_stream >> position_index;
                            face_stream >> texcoord_index;
                            face_stream >> normal_index;

                            glm::vec3 position = positions.at(position_index - 1);
                            glm::vec2 texcoord = texcoords.at(texcoord_index - 1);
                            glm::vec3 normal = normals.at(normal_index - 1);

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
            }
            model.vertex_count = vertices.size();
            {
                //create vertex buffer object on GPU
                glGenBuffers(1, &model.vbo);
                //set to current buffer 
                glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
                //copy data to current buffer
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
            }
        }
        {
            //create VAO
            glGenVertexArrays(1, &model.vao);
            //set as current vertex array
            glBindVertexArray(model.vao);
            GLsizei stride = 5 * sizeof(GLfloat);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);

           //edit this later
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
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
struct Camera
{
    glm::mat4 camera_from_world = glm::mat4(1);
    float fov = 60.0f;
    float near = 0.01f;
    float far = 1000.0f;
    glm::mat4 view_from_camera(GLFWwindow* window)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glViewport(0.0f, 0.0f, width, height);
        return glm::perspective(glm::radians(this->fov), float(width) / float(height), this->near, this->far);
        //GLint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
        //glUniformMatrix4fv(view_from_camera_location, 1, GL_FALSE, glm::value_ptr(view_from_camera));
    }
};
class Particle
{
public:
    glm::mat4 world_from_model;
    glm::vec3 velocity;
    Particle(glm::mat4 world_from_model, glm::vec3 velocity)
    {
        this->world_from_model = world_from_model;
        this->velocity = velocity;
    }
};
void load_textures()
{
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
    float pixels[]
    {
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels);
}
GLFWwindow* initialize_glfw()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        cout << "glfwINIT( ) failed \n";
        abort();
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        cout << "glfwCreateWindow failed \n";
        glfwTerminate();
        abort();
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    return window;
}
GLuint compiler_shader()
{
    //vertex shader
    const char* vertex_shader_src =
        "#version 330 core\n"
        "layout (location = 0) in vec3 pos;\n"
        "layout(location=1) in vec2 texcoords;\n"

        "layout(location=2) in vec3 normal;\n"
        
        "out vec2 Texcoords;\n"

        "out vec3 Normal;\n"

        "uniform vec2 offset;\n"
        "uniform mat4 camera_from_world;\n"
        "uniform mat4 view_from_camera;\n"
        "uniform mat4 world_from_model;\n"
        "void main(){\n"
        "Texcoords = texcoords;"
        "Normal = normal;"
        "gl_Position = view_from_camera*camera_from_world*world_from_model*vec4(pos, 1.0);\n"
        "}\n";
    //fragment shader
    const char* fragment_shader_src =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 Texcoords;\n"
        "uniform vec4 color;\n"
        "uniform sampler2D tex;\n"
        "void main(){\n"
        
        "   vec3 ambient = vec3("

            /*"FragColor = texture(tex, Texcoords);\n"*/
        "   float fog = gl_FragCoord.z / gl_FragCoord.w;"
        /*"   FragColor = vec4(0.05 * vec3(fog), 1.0);\n"*/
        "   FragColor = vec4(0.5 * (Normal + vec3(1.0)), 1.0);\n"
        "}\n";
    //compile vertex
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    //compile fragment
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    //link shaders
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    //delete shaders when done attaching
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glUseProgram(shader_program);
    return shader_program;
}
void render_scene(GLFWwindow* window, Model model, GLuint shader_program, vector<Particle>& particles, Camera camera)
{
    //set clear color
    glClearColor(0.7f, 0.0f, 0.5f, 1.0f);
    ////clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //camera from world
    GLint camera_from_world_location = glGetUniformLocation(shader_program, "camera_from_world");
    glUniformMatrix4fv(camera_from_world_location, 1, GL_FALSE, glm::value_ptr(camera.camera_from_world));
    //view from camera
    GLint view_from_camera_location = glGetUniformLocation(shader_program, "view_from_camera");
    glUniformMatrix4fv(view_from_camera_location, 1, GL_FALSE, glm::value_ptr(camera.view_from_camera(window)));
    //color location
    //GLint color_location = glGetUniformLocation(shader_program, "color");
    //glUniform4f(color_location, 0.0f, 0.9f, 1.0f, 1.0f);
    //texture location
    GLuint tex_location = glGetUniformLocation(shader_program, "tex");
    glUniform1i(tex_location, 0);
    for (Particle& particle : particles)
    {
        particle.world_from_model = glm::translate(particle.world_from_model, particle.velocity);
        particle.world_from_model = glm::rotate(particle.world_from_model, 0.0001f, glm::vec3(1.0f, 1.0f, 1.0f));
        GLint world_from_model_location = glGetUniformLocation(shader_program, "world_from_model");
        glUniformMatrix4fv(world_from_model_location, 1, GL_FALSE, glm::value_ptr(particle.world_from_model));
        glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        position = particle.world_from_model * position;
        model.draw();
    }
    //display the results on the screen
    glfwSwapBuffers(window);
}
void cleanup(GLuint* shader_program, Model model, GLuint tex)
{
    glDeleteProgram(*shader_program);
    model.cleanup();
    glDeleteTextures(1, &tex);
    glfwTerminate();
}
int main(void)
{
    GLFWwindow* window = initialize_glfw();
    GLuint shader_program = compiler_shader();
    Model model = Model::load();
    GLuint tex = 0;
    load_textures();
    vector<Particle> particles;
    //teapot 
    particles.push_back(Particle(
        glm::translate(glm::mat4(1), glm::vec3(-0.2f, -0.2f, 0.0f)),
        glm::vec3(0.0f, 0.0f, 0.0f)
        ));
    //cube
    Camera camera;
    //glm::mat4 camera_from_world = glm::mat4(1);
    camera.camera_from_world = glm::translate(camera.camera_from_world, glm::vec3(0.0f, 0.0f, -10.0f));
    //GLint world_to_camera_location = glGetUniformLocation(shader_program, "camera_from_world");
    while (!glfwWindowShouldClose(window))
    {
        //camera_from_world = glm::translate(camera_from_world, glm::vec3(.01f, 0.0f, 0.0f));
        //camera_from_world = glm::rotate(camera_from_world, 0.001f, glm::vec3(0.0f, 0.0f, 1.0f));
        //render scene
        render_scene(window, model, shader_program, particles, camera);
        glfwPollEvents();
    }
    cleanup(&shader_program, model, tex);
    return 0;
}