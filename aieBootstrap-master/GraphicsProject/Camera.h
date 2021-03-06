/*
Author:				Igor Doslov
Date created:       10/3/2021
Date modified:      7/4/2021
File:				Camera.h
Purpose:			Camera class used for creating a camera in the app
*/

#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera(bool a_isStatic, glm::vec3 a_camPos = glm::vec3(0)); // Takes in a bool that sets the camera to static or not
	Camera(glm::vec3 a_camPos = glm::vec3(0)); // Camera is dynamic by default

	~Camera() {};

	void Update(float a_deltaTime);

	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix(float a_width, float a_height);

	bool m_isStatic = true;
private:
	float m_theta; // In degrees
	float m_phi; // In degress
	glm::vec3 m_position;
	float m_speed = 10.f;
	float m_lastMouseX = 0.0f;
	float m_lastMouseY = 0.0f;
};

