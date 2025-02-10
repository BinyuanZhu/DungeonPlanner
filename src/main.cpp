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

// Button struct
typedef struct {
    float xMin, xMax, yMin, yMax;
} Button;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Butotn pixel locations
Button startButton = { 300, 500, 325, 375 };
Button exitButton = { 300, 500, 225, 275 };

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

    // Button vertices
    float buttonVertices[] = {
        // Start Button
        300.0f, 325.0f,
        500.0f, 325.0f,
        500.0f, 375.0f,
        500.0f, 375.0f,
        300.0f, 375.0f,
        300.0f, 325.0f,
        
        // Exit Button
        300.0f, 225.0f,
        500.0f, 225.0f,
        500.0f, 275.0f,
        500.0f, 275.0f,
        300.0f, 275.0f,
        300.0f, 225.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices), buttonVertices, GL_STATIC_DRAW);
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

        glBindVertexArray(VAO);

        // Draw Start Button (Blue)
        menuShader.setVec4("ourColor", glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw Exit Button (Red)
        menuShader.setVec4("ourColor", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 6, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Dynamic button logic
bool isButtonClicked(Button button, double xpos, double ypos, int width, int height) {
    float scaleX = (float)width / SCR_WIDTH;
    float scaleY = (float)height / SCR_HEIGHT;
    float adjXMin = button.xMin * scaleX;
    float adjXMax = button.xMax * scaleX;
    float adjYMin = button.yMin * scaleY;
    float adjYMax = button.yMax * scaleY;

    float ypos_gl = height - ypos;
    return (xpos >= adjXMin && xpos <= adjXMax && ypos_gl >= adjYMin && ypos_gl <= adjYMax);
}

// Mouse inputs
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        if (isButtonClicked(startButton, xpos, ypos, width, height)) {
            std::cout << "Starting game..." << std::endl;
        } else if (isButtonClicked(exitButton, xpos, ypos, width, height)) {
            glfwSetWindowShouldClose(window, true);
        }
    }
}

// Key inputs
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
