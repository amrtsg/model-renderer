#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <string>
#include <Camera.h>

using namespace std;

//Dimensions of Window
const static int WIN_WIDTH = 1100;
const static int WIN_HEIGHT = 600;

class Window {

public:
	Window();
	void specifyHints();
	void startGLFW();
	void startGLEW();
	void setupWindow();

public:
	int WIDTH;
	int HEIGHT;
	GLFWwindow* window;
};
