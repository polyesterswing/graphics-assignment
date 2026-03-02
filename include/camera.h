#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Orbit parameters
	float radius = 5.0f;
	float yaw = glm::radians(90.0f);
	float pitch = 0.0f;

	float mouseSensitivity = 0.005f;
	float zoomSensitivity = 1.0f;

	glm::vec3 target = glm::vec3(0.0f);

	Camera(glm::vec3 target, float radius): radius(radius), target(target) {}

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 getViewMatrix() const {
		
		glm::vec3 position;

		position.x = target.x + radius * cos(pitch) * cos(yaw);
        position.y = target.y + radius * sin(pitch);
        position.z = target.z + radius * cos(pitch) * sin(yaw);

        return glm::lookAt(position, target, glm::vec3(0, 1, 0));
    }

	void processMouseMovement(float dx, float dy) {
		yaw += dx * mouseSensitivity;
		pitch += dy * mouseSensitivity;

		pitch = std::clamp(pitch, glm::radians(-89.0f), glm::radians(89.0f));
	}

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset)
    {
		radius -= yoffset * zoomSensitivity;
		radius = std::clamp(radius, 1.0f, 100.0f);

    }

	void processMousePan(float dx, float dy)
	{
		glm::vec3 position;

		position.x = target.x + radius * cos(pitch) * cos(yaw);
		position.y = target.y + radius * sin(pitch);
		position.z = target.z + radius * cos(pitch) * sin(yaw);

		glm::vec3 forward = glm::normalize(target - position);
		glm::vec3 right   = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
		glm::vec3 up      = glm::normalize(glm::cross(right, forward));

		float panSpeed = 0.002f * radius; // scale with distance

		target -= right * dx * panSpeed;
		target += up    * dy * panSpeed;
	}

};
#endif
