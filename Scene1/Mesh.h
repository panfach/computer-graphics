#ifndef MESH_H
#define MESH_H
#define GLM_FORCE_RADIANS


#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

using namespace std;

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

enum TextureType {
    DIFFUSE = 0,
    SPECULAR = 1,
    NORMAL = 2,
    HEIGHT = 3
};

struct Texture {
    unsigned int id;
    TextureType type;
    string path;
};

class Mesh {
public:
    vector<Vertex>       vertices;
    vector<unsigned int> indices;
    vector<Texture>      textures;
    unsigned int VAO, VBO, EBO;

    Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures)
    {
        vertices = _vertices;
        indices = _indices;
        textures = _textures;

        Init();
    }

    void Draw(Shader& shader)
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;

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


            //glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
            glUniform1i(glGetUniformLocation(shader.id, (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

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
#endif