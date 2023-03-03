#pragma once
#include <GL/glew.h>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
    UP,
    DOWN
};

const GLfloat YAW = 0.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 15.0f;
const GLfloat SENSITIVITY = 0.25f;
const GLfloat FOV = 45.0f;

class Camera
{
public:
    glm::vec3 position;
    GLfloat yaw;
    GLfloat pitch;
    GLfloat fov;
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) 
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV) {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) 
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(FOV) {
        this->position = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
    }

    //return view matrix (in the direction camera is pointing)
    glm::mat4 GetViewMatrix();
    //Process input from keyboard
    void ProcessKeyboard(CameraMovement direction, GLfloat deltaTime);
    //Process input from mouse movement
    void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);
    //return camera FOV
    GLfloat GetFOV();
    //calculate forward vector from where camera is looking at
    void updateCameraVectors();
    //move camera
    void moveCamera(bool keys[1024], GLfloat deltaTime);
    //keyboard callback
    GLFWkeyfun KeyCallback(bool keys[1024], GLFWwindow* window, int key, int scancode, int action, int mode);
    //invert pitch of camera
    void invertPitch();
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
private:
    
};

