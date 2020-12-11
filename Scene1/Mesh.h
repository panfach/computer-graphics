#ifndef MESH_H
#define MESH_H
#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <vector>
#include <string>
#include "Shader.h"

using namespace std;

// Загрузка текстуры из файла filename в папке directory
static unsigned int LoadTexture(const string filename, const string& directory);

enum TextureType {
    DIFFUSE = 0,
    SPECULAR = 1,
    NORMAL = 2,
    HEIGHT = 3
};

enum MeshType {
    COMPLEX,
    PLANE
};

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    /*Vertex() {
        pos = glm::vec3(0.0f);
        normal = glm::vec3(0.0f, 1.0f, 0.0f);
        texCoord = glm::vec2(0.0f);
        tangent = glm::vec3(1.0f, 0.0f, 0.0f);
        bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
    }*/

    Vertex() {
        pos = glm::vec3(0.0f);
        normal = glm::vec3(0.0f);
        texCoord = glm::vec2(0.0f);
        tangent = glm::vec3(0.0f);
        bitangent = glm::vec3(0.0f);
    }

    Vertex(glm::vec3 _pos, glm::vec3 _norm = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2 _texCoord = glm::vec2(0.0f),
        glm::vec3 _tang = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 _bitang = glm::vec3(0.0f, 0.0f, 1.0f)) {
        pos = _pos;
        normal = _norm;
        texCoord = _texCoord;
        tangent = _tang;
        bitangent = _bitang;
    }
};

struct Texture {
    unsigned int id;
    TextureType type;
    string path;

    Texture() {
        id = 0;
        type = DIFFUSE;
        path = "";
    }

    Texture(TextureType _type, const string _path) {
        string filepath = _path.substr(0, _path.find_last_of('/'));
        string filename = _path.substr(_path.find_last_of('/') + 1);
        id = LoadTexture(filename, filepath);
        type = _type;
        path = filename;
    }

    Texture(unsigned int _id) {
        id = _id;
        type = DIFFUSE;
        path = "";
    }
};

// Плоскость из четырех вершин
static GLfloat planeVertices[] = {
     // Координаты         // Нормали          // Текстура   // Касательные
     0.5f,  0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
     0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
    -0.5f,  0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f
};

// Плоскость из четырех вершин, повернутая по другому
static GLfloat planeVertices2[] = {
    // Координаты         // Нормали          // Текстура   // Касательные
    0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
    0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
   -0.5f,  -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
   -0.5f,  0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 1.0f
};

static GLuint defaultIndices[] = { 0, 1, 3, 1, 2, 3 };


class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    bool hasNormal;
    unsigned int VAO, VBO, EBO;

    // Создание меша, используя готовые векторы с данными
    Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures, bool normalFlag = false) {
        vertices = _vertices;
        indices = _indices;
        textures = _textures;
        hasNormal = normalFlag;

        Init();
    }

    // Создание примитивного меша типа, описанного переменной MeshType
    Mesh(MeshType meshType, int textureAmount, Texture* texture) {
        int size;
        if (meshType == PLANE) {
            size = 14;
            for (int i = 0; i < 4; i++) {
                glm::vec3 _pos(planeVertices[i * size], planeVertices[i * size + 1], planeVertices[i * size + 2]);
                glm::vec3 _norm(planeVertices[i * size + 3], planeVertices[i * size + 4], planeVertices[i * size + 5]);
                glm::vec2 _texCoord(planeVertices[i * size + 6], planeVertices[i * size + 7]);
                glm::vec3 _tangent(planeVertices[i * size + 8], planeVertices[i * size + 9], planeVertices[i * size + 10]);
                glm::vec3 _bitangent(planeVertices[i * size + 11], planeVertices[i * size + 12], planeVertices[i * size + 13]);
                vertices.push_back(Vertex(_pos, _norm, _texCoord, _tangent, _bitangent));
            }
            for (int i = 0; i < 6; i++) {
                indices.push_back(defaultIndices[i]);
            }
            for (int i = 0; i < textureAmount; i++) {
                if (texture[i].type == NORMAL) {
                    hasNormal = true;
                    cout << "This plane has normal map ..." << endl;
                }
                textures.push_back(texture[i]);
            }
        }

        Init();
    }

    Mesh(unsigned int texture) {
        int size = 14;
        for (int i = 0; i < 4; i++) {
            glm::vec3 _pos(planeVertices2[i * size], planeVertices2[i * size + 1], planeVertices2[i * size + 2]);
            glm::vec3 _norm(planeVertices2[i * size + 3], planeVertices2[i * size + 4], planeVertices2[i * size + 5]);
            glm::vec2 _texCoord(planeVertices2[i * size + 6], planeVertices2[i * size + 7]);
            vertices.push_back(Vertex(_pos, _norm, _texCoord));
        }
        for (int i = 0; i < 6; i++) {
            indices.push_back(defaultIndices[i]);
        }
        for (int i = 0; i < 6; i++) {
            indices.push_back(defaultIndices[i]);
        }
        textures.push_back(Texture(texture));

        Init();
    }

    void Draw(Shader& shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

        shader.SetInt("hasNormalMap", (hasNormal) ? 1 : 0);
        
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            
			glActiveTexture(GL_TEXTURE0 + i);
			// retrieve texture number (the N in diffuse_textureN)
			string number;
			string name;
			TextureType type = textures[i].type;
			switch (type) {
			case DIFFUSE:
				number = to_string(diffuseNr++);
				name = "diffuseTexture";
				break;
			case SPECULAR:
				number = to_string(specularNr++);
				name = "specularTexture";
				break;
			case NORMAL:
				number = to_string(normalNr++);
				name = "normalTexture";
				break;
			case HEIGHT:
				number = to_string(heightNr++);
				name = "heightTexture";
				break;
			}


            //if (name == "texture_diffuse")
            //    number = std::to_string(diffuseNr++);
            //else if (name == "texture_specular")
            //    number = std::to_string(specularNr++); // transfer unsigned int to stream
            //else if (name == "texture_normal")
            //    number = std::to_string(normalNr++); // transfer unsigned int to stream
            //else if (name == "texture_height")
            //    number = std::to_string(heightNr++); // transfer unsigned int to stream


            // Назначение uniform переменной в фрагментном шейдере
            glUniform1i(glGetUniformLocation(shader._program, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glActiveTexture(GL_TEXTURE0);
    }

private:
    void Init()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
        glBindVertexArray(0);
    }
};

static unsigned int LoadTexture(const string filename, const string& directory) {
    int width, height, nrComponents;
    string path = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);
    cout << "Moedl.h LoadTextureFromFile() textureId = " << textureID << endl;

    unsigned char* data = SOIL_load_image(path.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else // nrComponents == 4
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        std::cout << "Texture can not load texture: " << path << std::endl;
        SOIL_free_image_data(data);
    }

    return textureID;
}
#endif