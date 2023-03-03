#pragma once
#include <GL/glew.h>
#include <vector>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Light
{
public:

	Light(glm::vec3 position, glm::vec3 colour);
	glm::vec3 getPosition();
	glm::vec3 getColour();

private:
	glm::vec3 position;
	glm::vec3 colour;

};

