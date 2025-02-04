/* Ask for an OpenGL Core Context */
#define GLFW_INCLUDE_NONE
#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <filesystem>
#include <iostream>
#include "shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace fs = std::filesystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
    // Init 
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Main Menu", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // Load GLLoader
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Get relative path
    fs::path projectRoot = fs::current_path();
    Shader menuShader((projectRoot / "src/shaders/vertex.glsl").c_str(), 
                      (projectRoot / "src/shaders/fragment.glsl").c_str());

    // Button 1 (Start) vertices
    float button1Vertices[] = {
        300.0f, 325.0f,  // Bottom-left
        500.0f, 325.0f,  // Bottom-right
        500.0f, 375.0f,  // Top-right
        500.0f, 375.0f,  // Top-right
        300.0f, 375.0f,  // Top-left
        300.0f, 325.0f   // Bottom-left
    };

    // Button 2 (Exit) vertices
    float button2Vertices[] = {
        300.0f, 225.0f,
        500.0f, 225.0f,
        500.0f, 275.0f,
        500.0f, 275.0f,
        300.0f, 275.0f,
        300.0f, 225.0f
    };

    // Create VAO and VBO
    unsigned int VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    // Start Button
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(button1Vertices), button1Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Exit Button
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(button2Vertices), button2Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
    // Loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        menuShader.use();
        menuShader.setMat4("projection", projection);

        // Draw Start Button (Blue)
        menuShader.setVec4("ourColor", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw Exit Button (Red)
        menuShader.setVec4("ourColor", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glfwTerminate();
    return 0;
}

// Mouse inputs
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float ypos_gl = SCR_HEIGHT - ypos;

        // Start Button Click (300-500x, 325-375y) --> Needs to be dynamic
        if (xpos >= 300 && xpos <= 500 && ypos_gl >= 325 && ypos_gl <= 375) {
            std::cout << "Starting game..." << std::endl;
            // Add game initialization here
        }
        // Exit Button Click (300-500x, 225-275y) --> Unecesary, ESC to exit
        else if (xpos >= 300 && xpos <= 500 && ypos_gl >= 225 && ypos_gl <= 275) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

// Key inputs
void processInput(GLFWwindow *window) {
    // ESC to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}