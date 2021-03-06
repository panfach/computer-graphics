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

using namespace std;

class Shader {
public:
    GLuint id;
	GLuint _program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		string vertexCode;
		string fragmentCode;
		ifstream vertexShaderFile;
		ifstream fragmentShaderFile;
        id = 0;

        try {

            vertexShaderFile.open(vertexPath);
            fragmentShaderFile.open(fragmentPath);
            stringstream vertexShaderStream, fragmentShaderStream;

            vertexShaderStream << vertexShaderFile.rdbuf();
            fragmentShaderStream << fragmentShaderFile.rdbuf();

            vertexShaderFile.close();
            fragmentShaderFile.close();

            vertexCode = vertexShaderStream.str();
            fragmentCode = fragmentShaderStream.str();
        }
        catch (ifstream::failure e) {
            cout << "Shader error. file " << vertexPath << " or " << fragmentPath << "not successully read" << endl;
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
            cout << "Shader error. " << vertexPath << " compilation failed\n" << infoLog << endl;
        };

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragment);

        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            cout << "Shader error. " << fragmentPath << " compilation failed\n" << infoLog << endl;
        }

        _program = glCreateProgram();
        glAttachShader(_program, vertex);
        glAttachShader(_program, fragment);
        glLinkProgram(_program);

        glGetProgramiv(_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(_program, 512, NULL, infoLog);
            cout << "Shader error. Program linking failed\n" << infoLog << endl;
        }

        glDeleteShader(vertex);
        glDeleteShader(fragment);
	}

    void SetInt(const char* variableName, int value) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniform1i(loc, value); 
    }

    void SetFloat(const char* variableName, float value) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniform1f(loc, value);
    }

    void SetVec3(const char* variableName, glm::vec3 &value) {
        SetVec3(variableName, value.x, value.y, value.z);
    }

    void SetVec3(const char* variableName, float x, float y, float z) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniform3f(loc, x, y, z);
    }

    void SetMat4(const char* variableName, glm::mat4 &value) {
        GLint loc = glGetUniformLocation(_program, variableName);
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Use() {
        glUseProgram(_program);
    }
};

#endif

