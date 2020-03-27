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
                            istringstream face_stream(face);
                            face_stream >> position_index;
                            face_stream >> texcoord_index;
                            glm::vec3 position = positions.at(position_index - 1);
                            glm::vec2 texcoord = texcoords.at(texcoord_index - 1);
                            vertices.push_back(position.x);
                            vertices.push_back(position.y);
                            vertices.push_back(position.z);
                            vertices.push_back(texcoord.x);
                            vertices.push_back(texcoord.y);
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
        "out vec2 Texcoords;\n"
        "uniform vec2 offset;\n"
        "uniform mat4 camera_from_world;\n"
        "uniform mat4 view_from_camera;\n"
        "uniform mat4 world_from_model;\n"
        "void main(){\n"
        "Texcoords = texcoords;"
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
        "FragColor = texture(tex, Texcoords);\n"
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
        particle.world_from_model = glm::rotate(particle.world_from_model, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
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
        glm::translate(glm::mat4(1), glm::vec3(-0.5f, -0.5f, 0.0f)),
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