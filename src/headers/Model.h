#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm.hpp>
#include <Mesh.h>
#include <Texture.h>

using namespace std;

class Model
{
public:
    Model(string path, glm::vec3 position, glm::vec3 size, glm::vec3 rotation);

    void draw(Shader* shader);
    GLint TextureFromFile(const char* path, string directory);
    Shader* getShader();
    glm::mat4 getModelMatrix();
    glm::vec3 position, size, rotation;

private:
    vector<Mesh> meshes;
    string directory;
    vector<TextureInfo> textures_loaded;

    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    Shader* shader;
};
