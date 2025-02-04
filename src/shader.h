#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        try
        {
            std::cout << "Attempting to open vertex shader: " << vertexPath << std::endl;
            std::cout << "Attempting to open fragment shader: " << fragmentPath << std::endl;
            // Compile shaders
            unsigned int vertex = shaderFromFile(vertexPath, GL_VERTEX_SHADER);
            unsigned int fragment = shaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);

            // Link shaders
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);

            // Check for linking errors
            GLint success;
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                GLchar infoLog[512];
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                throw std::runtime_error(std::string("ERROR::SHADER::PROGRAM::LINKING_FAILED\n") + infoLog);
            }

            // Delete shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setVec2(const std::string &name, float x, float y) const
    {
        float value[2] = {x, y};
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value);
    }
    
    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void setChar(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    private:
        int shaderFromFile(const std::string& filePath, GLenum shaderType) {
            //open file
            std::ifstream f;
            f.open(filePath.c_str(), std::ios::in);
            if(!f.is_open()){
                throw std::runtime_error(std::string("Failed to open file: ") + filePath);
            }

            //read whole file into stringstream buffer
            std::stringstream buffer;
            buffer << f.rdbuf();
            f.close();
            std::string shaderCode = buffer.str();

            //create new shader
            int ShaderID = glCreateShader(shaderType);

            //set the source code
            const GLchar* code = shaderCode.c_str();
            glShaderSource(ShaderID, 1, &code, NULL);

            //compile
            glCompileShader(ShaderID);

            //check for compile errors
            GLint status;
            glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &status);
            std::cout << "Status from compile:" << status << "\r\n";
            if (status == GL_FALSE) {
                std::string msg("Compile failure in shader:\n");

                GLint infoLogLength;
                glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
                char* strInfoLog = new char[infoLogLength + 1];
                glGetShaderInfoLog(ShaderID, infoLogLength, NULL, strInfoLog);
                msg += strInfoLog;
                delete[] strInfoLog;

                glDeleteShader(ShaderID); ShaderID = 0;
                throw std::runtime_error(msg);
            }

            return ShaderID;
        }
    };
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    };

#endif
