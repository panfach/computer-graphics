#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint id;
	GLuint _program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

        id = 0;

		vertexShaderFile.exceptions(std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit);

        try {

            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            std::stringstream vertexShaderStream, fragmentShaderStream;

            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexCode = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }

        const GLchar* vertexShaderCode = vertexCode.c_str();
        const GLchar* fragmentShaderCode = fragmentCode.c_str();

        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexShaderCode, NULL);
        glCompileShader(vertex);

        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        _program = glCreateProgram();
        glAttachShader(_program, vertex);
        glAttachShader(_program, fragment);
        glLinkProgram(_program);

        glGetProgramiv(_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(_program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

    void SetInt(const char* variableName, int value) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniform1i(loc, value); //////////
    }

    void SetFloat(const char* variableName, float value) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniform1f(loc, value);
    }

    void SetVec3(const char* variableName, glm::vec3 value) {
        SetVec3(variableName, value.x, value.y, value.z);
    }

    void SetVec3(const char* variableName, float x, float y, float z) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniform3f(loc, x, y, z);
    }

    void Use() {
        glUseProgram(_program);
    }
};

#endif

