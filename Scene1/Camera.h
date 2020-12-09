#ifndef CAMERA_H
#define CAMERA_H
#define GLM_FORCE_RADIANS

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

enum CameraDirection {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	GLfloat cameraSpeed = 5.0f;
	GLfloat sensitivity = 0.05f;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldFront;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;
	GLfloat fov;

	Camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 3.0f), 
		   glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), 
		   glm::vec3 _front = glm::vec3(0.0f, 0.0f, -1.0f),
		   GLfloat _yaw = -90.0f, GLfloat _pitch = 0.0f, GLfloat _fov = 45.0f) {

		position = _position;
		worldFront = _front;
		worldUp = _up;

		yaw = _yaw;
		pitch = _pitch;
		fov = _fov;

		std::cout << "fov = " << fov << endl;

		RecalculateVectors();
	}

	void Move(CameraDirection dir, GLfloat deltaTime) {
		GLfloat velocity = cameraSpeed * deltaTime;
		if (dir == FORWARD)
			position += front * velocity;
		if (dir == BACKWARD)
			position -= front * velocity;
		if (dir == LEFT)
			position -= right * velocity;
		if (dir == RIGHT)
			position += right * velocity;
	}

	void Rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch -= yoffset;

		if (constrainPitch)
		{
			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;
		}

		RecalculateVectors();
	}

	void Zoom(bool state) {
		if (state)
			fov = 15.0f;
		else
			fov = 75.0f;
	}

private:
	void RecalculateVectors() {
		glm::vec3 _front;
		_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		_front.y = sin(glm::radians(pitch));
		_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

		front = glm::normalize(_front);
		right = glm::normalize(glm::cross(front, worldUp));
		up = glm::normalize(glm::cross(right, front));
	}
};
#endif