#ifndef MODEL_H
#define MODEL_H
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

using namespace std;

class Model {
public:
    vector<Texture> textureCache;
    vector<Mesh> meshes;
    string directory;

    Model(const char* _path) {
        LoadModel(_path);
    }
    

    void Draw(Shader shader) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    void LoadModel(const string &path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            cout << "ERROR: ASSIMP " << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }

    void ProcessNode(aiNode* node, const aiScene* scene) {

        // Заполнение meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene));
            //cout << "Model.h ProcessNode() mesh have " << meshes[0].textures.size() << " textures." << endl;
        }

        // Рекурсивная обработка дочерних мешей
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }

        //cout << "Model.h ProcessNode() meshes.size = " << meshes.size() << endl;
    }

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
        bool hasNormal = false;

        // Заполнение vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			glm::vec3 vector;

			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.pos = vector;

            // Если есть нормали - заполнение
            if (mesh->HasNormals()) {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
            }
            else 
                vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);

			if (mesh->mTextureCoords[0]) {
                glm::vec2 texVec;

				texVec.x = mesh->mTextureCoords[0][i].x;
				texVec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoord = texVec;

                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.tangent = vector;

                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.bitangent = vector;
			}
			else
				vertex.texCoord = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}

        // Заполнение indices (Информация о гранях)
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, NORMAL);
        if (normalMaps.size() > 0) hasNormal = true;
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		return Mesh(vertices, indices, textures, hasNormal);
	}

    vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType) {
        vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString sourceName;
            mat->GetTexture(type, i, &sourceName); 
            
            // Проверка того, есть ли уже такая текстура в textureCache
            bool skip = false;
            for (unsigned int j = 0; j < textureCache.size(); j++) {
                if (strcmp(textureCache[j].path.data(), sourceName.C_Str()) == 0) {
                    textures.push_back(textureCache[j]);
                    skip = true; 
                    break;
                }
            }

            // Оптимизация
            if (!skip) {
                Texture texture;
                texture.id = LoadTexture(sourceName.C_Str(), this->directory);
                texture.type = texType;
                texture.path = sourceName.C_Str();
                textures.push_back(texture);
                textureCache.push_back(texture);
            }
        }
        return textures;
    }
};

#endif