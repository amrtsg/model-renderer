#pragma once

#include <vector>
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

using namespace std;

class Texture
{
public:
	Texture();
	Texture(unsigned int texture);
	unsigned int loadTexture(const char* path);
	int getID();
	unsigned int textureID;
};

