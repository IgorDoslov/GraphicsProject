#include "Camera.h"
#include <glm/ext.hpp>
#include <Input.h>

Camera::Camera(bool a_isStatic, glm::vec3 a_camPos)
{
	m_position = a_camPos;
	m_phi = 0;
	m_theta = 0;
	m_isStatic = a_isStatic;
}
Camera::Camera(glm::vec3 a_camPos) : m_lastMouseX(), m_lastMouseY()
{
	m_position = a_camPos;
	m_phi = 0;
	m_theta = 0;
	
}

void Camera::Update(float a_deltaTime)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	// Calculate the forwards and right axes and the up axis for the camera
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0, 1, 0);

#pragma region Input Movement

	// move camera up
	if (input->isKeyDown(aie::INPUT_KEY_Q))
		m_position += up * a_deltaTime * m_speed;

	// move camera down
	if (input->isKeyDown(aie::INPUT_KEY_E))
		m_position -= up * a_deltaTime * m_speed;

	// move camera left
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position -= right * a_deltaTime * m_speed;

	// move camer right
	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * a_deltaTime * m_speed;

	// move camera forward
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * a_deltaTime * m_speed;

	// move camera back
	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * a_deltaTime * m_speed;

#pragma endregion

	// Get the current position of the mouse coordinates
	float mX = (float)input->getMouseX();
	float mY = (float)input->getMouseY();
	constexpr float turnSpeed = glm::radians(180.f) * 4.f;

	// If the right button is down, increment the theta and phi
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += turnSpeed * (mX - m_lastMouseX) * a_deltaTime;
		m_phi += turnSpeed * (mY - m_lastMouseY) * a_deltaTime; // Change + to - to invert Y controls
	}

	// Now store the frames last values for the next
	m_lastMouseX = mX;
	m_lastMouseY = mY;

}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	// If static camera == false then move, otherwise look at the centre
	if (m_isStatic == false)
		return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
	else
		return glm::lookAt(m_position, glm::vec3(0) + forward, glm::vec3(0, 1, 0));

}

glm::mat4 Camera::GetProjectionMatrix(float a_width, float a_height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, a_width / a_height, 0.1f, 1000.0f);
}
