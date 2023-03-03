#include <Mesh.h>

void Mesh::draw(Shader* shader) {

	GLuint numOfDiffTex = 1;
	GLuint numOfSpecTex = 1;

	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		stringstream ss;
		string number;
		string name = this->textures[i].type;
		if (name == "texture_diffuse") {
			ss << numOfDiffTex++;
		}
		else if (name == "texture_specular") {
			ss << numOfSpecTex++;
		}
		number = ss.str();
		glUniform1i(glGetUniformLocation(shader->Program, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}
	glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), 16.0f);
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//set all back to default
	for (GLuint i = 0; i < this->textures.size(); i++){
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::setupMesh() {
		glGenVertexArrays(1, &this->vao);
		glGenBuffers(1, &this->vbo);
		glGenBuffers(1, &this->ebo);
		glBindVertexArray(this->vao);
		//specify array of vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexInfo), &this->vertices[0], GL_STATIC_DRAW);
		//specify array of indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
		//specify pointers in array for positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)0);
		//specify pointers in array for normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, normal));
		//specify pointers in array for tex coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexInfo), (GLvoid*)offsetof(VertexInfo, texCoords));
		//unbind vao
		glBindVertexArray(0);
	}