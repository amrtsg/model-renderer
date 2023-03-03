#include <Window.h>

Window::Window() {
	this->WIDTH = WIN_WIDTH;
	this->HEIGHT = WIN_HEIGHT;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Model Renderer & Screenshot", NULL, NULL);
	startGLFW();
	specifyHints();
}

void Window::specifyHints() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //using openGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

void Window::startGLFW() {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW" << endl;
	}
}

void Window::startGLEW() {
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwTerminate();
	}
}

void Window::setupWindow() {
	glfwGetFramebufferSize(window, &WIDTH, &HEIGHT);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//hide cursor
	glfwMakeContextCurrent(window);
	startGLEW();
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);
}