#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <Texture.h>
#include <Window.h>
#include <Renderer.h>
#include <random>

#include <Windows.h>
#include <shobjidl.h>

using namespace std;



string sSelectedFile;
string sFilePath;
const int viewPortXStart = 300;

void saveImage(char* filepath, GLFWwindow* w) {
	int width, height;
	glfwGetFramebufferSize(w, &width, &height);
	GLsizei nrChannels = 3;
	GLsizei stride = nrChannels * width;
	stride += (stride % 4) ? (4 - stride % 4) : 0;
	GLsizei bufferSize = stride * height;
	std::vector<char> buffer(bufferSize);
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	glReadBuffer(GL_FRONT);
	glReadPixels(viewPortXStart, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
	stbi_flip_vertically_on_write(true);
	stbi_write_png(filepath, 800, 600, nrChannels, buffer.data(), stride);
}

// Function prototypes
bool openFile() {
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return FALSE;
	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}
	//  SHOW OPEN FILE DIALOG WINDOW
	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}
	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}
	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}
	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	sFilePath = c;
	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = sFilePath.find_last_of("/\\");
	sSelectedFile = sFilePath.substr(slash + 1);
	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
//Scene dimensions
const int WIDTH = 800;
const int HEIGHT = 600;
Window win;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
Camera camera(glm::vec3(-50, 10, 0));
bool firstMouse = true;
bool keys[1024];
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
vector<Model> models;

int main() {
	//setup and create window
	Window win;
	win.setupWindow();
	glViewport(viewPortXStart, 0, WIDTH, HEIGHT);
	//glfw input callbacks

	Light light(glm::vec3(-100.0, 100.0, -100.0), glm::vec3(1, 1, 1));
	Renderer renderer;

	//setup objects
	Model house("..\\..\\res\\models\\cottage\\house.obj", 
		glm::vec3(0.0f, 0.0f, 0.0f), //pos
		glm::vec3(1.0f, 1.0f, 1.0f), //scale
		glm::vec3(0.0f, 0.0f, 0.0f));//rot

	//SETUP IMGUI
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(win.window, true);
	ImGui::StyleColorsDark();
	
	//Setup callbacks (mouse and keyboard controls)
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glfwSetKeyCallback(win.window, KeyCallback);
	glfwSetCursorPosCallback(win.window, MouseCallback);
	glfwSetScrollCallback(win.window, ScrollCallback);

	bool GUIActive = true;
	glm::vec4 backColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	int counter = 0;

	static bool no_titlebar = true;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = true;
	static bool no_resize = true;
	static bool no_collapse = true;
	static bool no_close = true;
	static bool no_nav = true;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//Main gameloop
	while (!glfwWindowShouldClose(win.window)) {
		//camera.yaw = 50;
		house.rotation.y += 0.5f;
		if (house.rotation.y == 360) {
			house.rotation.y = 0;
		}
		// Demonstrate the various window flags. Typically you would just use the default.
		ImGuiWindowFlags window_flags = 0;
		if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
		if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
		if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
		if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
		if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
		if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
		if (no_nav)       window_flags |= ImGuiWindowFlags_NoNav;

		ImGui::SetNextWindowSize(ImVec2(300, win.HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui_ImplGlfwGL3_NewFrame();
		//BEGIN IMGUI ------------------------------------------------------------------------
		ImGui::Begin("GUI", &GUIActive, window_flags);
		ImGui::GetStyle().WindowRounding = 0.0f;
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open")) {
					if (openFile()) {
						printf("SELECTED FILE: %s\nFILE PATH: %s\n\n", sSelectedFile.c_str(), sFilePath.c_str());
						house = Model(sFilePath,
							glm::vec3(0.0f, 0.0f, 0.0f), //pos
							glm::vec3(1.0f, 1.0f, 1.0f), //scale
							glm::vec3(0.0f, 0.0f, 0.0f));
					}
					else {
						printf("ERROR");
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Model");
		ImGui::DragFloat3("Position", (float*) &house.position);
        ImGui::DragFloat3("Rotate", (float*) &house.rotation);
		ImGui::DragFloat3("Scale", (float*) &house.size);
		ImGui::ColorEdit3("Background", (float*) &backColor);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Camera");
		ImGui::DragFloat3("Cam Position", (float*) &camera.position);
		ImGui::DragFloat("Cam Yaw", &camera.yaw);
		ImGui::DragFloat("Cam Pitch", &camera.pitch);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Settings");
		ImGui::Checkbox("No titlebar", &no_titlebar); ImGui::SameLine(150);
		ImGui::Checkbox("No scrollbar", &no_scrollbar);
		ImGui::Checkbox("No menu", &no_menu); ImGui::SameLine(150);
		ImGui::Checkbox("No move", &no_move);
		ImGui::Checkbox("No resize", &no_resize); ImGui::SameLine(150);
		ImGui::Checkbox("No collapse", &no_collapse); 
		ImGui::Checkbox("No close", &no_close); ImGui::SameLine(150);
		ImGui::Checkbox("No nav", &no_nav);
		if (ImGui::Button("Screenshot")) {
			saveImage("../../screenshots/imgsave.png", win.window);
		}
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Metrics");
		ImGui::Text("AVG %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		//END IMGUI ----------------------------------------------------------------------------------------------------------------------------------------------

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//cout << camera.position.x << " " << camera.position.y << " " << camera.position.z << endl;
		camera.moveCamera(keys, deltaTime);
		//background color
		glClearColor(backColor[0], backColor[1], backColor[2], backColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//enable clip distance
		glEnable(GL_CLIP_DISTANCE0);
		//create VP Matrices
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.fov, (float) WIDTH / HEIGHT, 0.1f, 1000.0f);
		//render to screen
		glDisable(GL_CLIP_DISTANCE0);
		//^ this line doesn't work on some graphics cards so we set the height to a very high number to be safe.
		//render object
		house.getShader()->use();
		house.getShader()->setUniforms(house.getModelMatrix(), view, projection);
		house.getShader()->loadClipPlane(glm::vec4(0, 1, 0, 99999999));
		house.draw(house.getShader());

	


		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(win.window);
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

//GLFW Handle key pressed
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			keys[key] = false;
		}
	}
}

//GLFW Handle mouse movements
void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
	GLfloat xOffset = 0;
	GLfloat yOffset = 0;
	if (state == GLFW_PRESS) {

		if (firstMouse) {
			lastX = xPos;
			lastY = yPos;
			firstMouse = false;
		}

		xOffset = xPos - lastX;
		yOffset = lastY - yPos;  //y-coordinates go from bottom to left SO REVERSE IT

		lastX = xPos;
		lastY = yPos;

	}
	camera.ProcessMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	float speed = 5;
	camera.position -= camera.front * (float) xoffset * speed;
	camera.position += camera.front * (float) yoffset * speed;
}
