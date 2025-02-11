/* Ask for an OpenGL Core Context */
#define GLFW_INCLUDE_NONE  // Macro Redefinition
#define GL_SILENCE_DEPRECATION  // THERE IS A REASON I AM USING 3.3

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <filesystem>
#include <iostream>

#include "shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>

namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const int GRID_SIZE = 10;
const int CELL_SIZE = 72;
const int GRID_WIDTH = GRID_SIZE * CELL_SIZE;
const int GRID_HEIGHT = GRID_SIZE * CELL_SIZE;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Dungeon Planner", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Get relative path
    fs::path projectRoot = fs::current_path();
    // Create and compile shaders. First concatenate paths, then convert to string via .c_str
    Shader gridShader((projectRoot / "src/shaders/vertex.glsl").c_str(), (projectRoot / "src/shaders/fragment.glsl").c_str());

    // VBO and VAO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    std::vector<float> vertices;
    for (int i = 0; i <= GRID_SIZE; ++i)
    {
        // Horizontal lines
        vertices.push_back(0.0f);
        vertices.push_back(i * CELL_SIZE);
        vertices.push_back(GRID_WIDTH);
        vertices.push_back(i * CELL_SIZE);

        // Vertical lines
        vertices.push_back(i * CELL_SIZE);
        vertices.push_back(0.0f);
        vertices.push_back(i * CELL_SIZE);
        vertices.push_back(GRID_HEIGHT);
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection =  glm::ortho(0.0f, (float)GRID_WIDTH, 0.0f, (float)GRID_HEIGHT);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // black out screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        gridShader.use();
        gridShader.setMat4("projection", projection);

        // Set viewport to center the grid
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Calculate viewport offsets to center the grid
        int viewportX = (windowWidth - GRID_WIDTH) / 2;
        int viewportY = (windowHeight - GRID_HEIGHT) / 2;

        glViewport(viewportX, viewportY, GRID_WIDTH, GRID_HEIGHT);

        // Draw grid
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 2);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// CALLBACKS

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
