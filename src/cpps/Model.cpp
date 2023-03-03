#include "Model.h"
#include <stb_image.h>

using namespace std;

Model::Model(string path, glm::vec3 position, glm::vec3 size, glm::vec3 rotation) {
	this->loadModel(path);
	this->position = position;
	this->size = size;
	this->rotation = rotation;
	this->shader = new Shader("..\\..\\src\\shaders\\modelVert.glsl", "..\\..\\src\\shaders\\modelFrag.glsl");
}

Shader* Model::getShader() {
	return this->shader;
}

//draw model
void Model::draw(Shader* shader) {
	for (GLuint i = 0; i < this->meshes.size(); i++) {
		this->meshes[i].draw(shader);
	}
}

glm::mat4 Model::getModelMatrix() {
	glm::vec3 xaxis = glm::vec3(1, 0, 0);
	glm::vec3 yaxis = glm::vec3(0, 1, 0);
	glm::vec3 zaxis = glm::vec3(0, 0, 1);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, this->position);
	model = glm::scale(model, this->size);
	model = glm::rotate(model, glm::radians(rotation.x), xaxis);
	model = glm::rotate(model, glm::radians(rotation.y), yaxis);
	model = glm::rotate(model, glm::radians(rotation.z), zaxis);
	return model;
}
//load model file using Assimp
void Model::loadModel(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));
	this->processNode(scene->mRootNode, scene);
}

//process each mesh
void Model::processNode(aiNode* node, const aiScene* scene) {
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

//process mesh again, uses temporary vector to store the values in assimps vector
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<VertexInfo> vertices;
	vector<GLuint> indices;
	vector<TextureInfo> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		VertexInfo vertex;
		glm::vec3 vector; // placeholder, assimp uses its own vectors

		//store assimps position vector values into the temp vector3
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//store assimps normal vector values into the temp vector3
		if (mesh->mNormals != NULL) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		//store assimps texture coord values into the temp vector2
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		} else {
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	//get vertex indices for each mesh
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		//store indices in vector
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<TextureInfo> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<TextureInfo> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	//return the mesh with all our processed vectors
	return Mesh(vertices, indices, textures);
}

//load texture materials, uses bunch of assimps predefined types, loads any unloaded textures
vector<TextureInfo> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<TextureInfo> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		GLboolean skip = false;

		//if texture is already loaded, then skip it
		for (GLuint j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j].path == str) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		//if texuture is not loaded then load it
		if (!skip) {
			TextureInfo texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);
		}
	}

	//return loaded textures in an array of TextureInfo structs
	return textures;
}

GLint Model::TextureFromFile(const char* path, string directory) {
	//get file
	string filename = string(path);
	directory = directory.substr(0, directory.find_last_of("\\/"));
	filename = directory + '/' + filename;
	//print if failure
	if (stbi_failure_reason()) cout << stbi_failure_reason();

	//get textureID
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	unsigned char* image = stbi_load(filename.c_str(), &width, &height, 0, STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//wrap tex
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);

	return textureID;
}