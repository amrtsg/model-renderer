/*
Renderer.h
*/

#pragma once

#include <Model.h>
#include <Camera.h>
#include <Light.h>

using namespace std;

class Renderer
{
public:
	//render all objects to scene
	void renderScene(Light light, vector<Model> entities, glm::mat4 view, glm::mat4 projection, Camera camera, glm::vec4 clipPlane);

};

