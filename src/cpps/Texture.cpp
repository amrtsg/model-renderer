#include "Texture.h"
#include <stb_image.h>


Texture::Texture(unsigned int texture) {
	this->textureID = texture;
}

int Texture::getID() {
	return textureID;
}

unsigned int Texture::loadTexture(const char* path) {
	//declare and bind texture
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//load texture image
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb_alpha);
	if (data)
	{
		//create a texture opengl object
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	else
	{
		cout << "Texture failed to load at path: " << path << endl;
		stbi_image_free(data);
	}
	
	//generate mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
}