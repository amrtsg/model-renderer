#pragma once


#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

//assimp
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include <Shader.h>

using namespace std;

struct VertexInfo {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct TextureInfo {
    GLuint id;
    string type;
    aiString path;
};

class Mesh {
public:

    vector<VertexInfo> vertices;
    vector<GLuint> indices;
    vector<TextureInfo> textures;

    Mesh(vector<VertexInfo> vertices, vector<GLuint> indices, vector<TextureInfo> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->setupMesh();
    }

    void draw(Shader* shader);

private:
    GLuint vao, vbo, ebo;
    void setupMesh();
};