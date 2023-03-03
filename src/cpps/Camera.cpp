#include <Camera.h>

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(this->position, this->position + this->front, this->up);
}

void Camera::ProcessKeyboard(CameraMovement direction, GLfloat deltaTime) {
    GLfloat velocity = this->movementSpeed * deltaTime;

    if (direction == FORWARD) {
        this->position += this->front * velocity;
    } else if (direction == BACKWARD) {
        this->position -= this->front * velocity;
    } else if (direction == LEFT) {
        this->position -= this->right * velocity;
    } else if (direction == RIGHT) {
        this->position += this->right * velocity;
    } else if (direction == UP) {
        this->position += up * velocity;
    } else if (direction == DOWN) {
        this->position -= up * velocity;
    }
}

void Camera::ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch) {
    xOffset *= this->mouseSensitivity;
    yOffset *= this->mouseSensitivity;

    this->yaw += xOffset;
    this->pitch += yOffset;

    if (constrainPitch) {
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        } else if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }

    this->updateCameraVectors();
}

GLfloat Camera::GetFOV() {
    return this->fov;
}

/*Updates where camera is pointing so that movement is relative to Camera*/
void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

void Camera::invertPitch() {
    this->pitch = -pitch;
    this->updateCameraVectors();
}

// Moves/alters the camera positions based on user input
void Camera::moveCamera(bool keys[1024], GLfloat deltaTime)
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        this->ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        this->ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        this->ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        this->ProcessKeyboard(RIGHT, deltaTime);
    }

    if (keys[GLFW_KEY_SPACE]) {
        this->ProcessKeyboard(UP, deltaTime);
    }

    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        this->ProcessKeyboard(DOWN, deltaTime);
    }

}