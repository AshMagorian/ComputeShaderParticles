#include "Camera.h"
#include <iostream>

void Camera::UpdateMatrix(int _dx, int _dy)
{
	m_pitch += _dy / 10.0f;
	m_yaw += _dx / 10.0f;

	if (m_pitch > 89.0f) { m_pitch = 89.0f; }
	if (m_pitch < -89.0f) { m_pitch = -89.0f; }

	if (m_yaw > 360.0f) { m_yaw = 0.0f; }
	if (m_yaw < -360.0f) { m_yaw = 0.0f; }

	glm::mat4 tempMat = glm::mat4(1.0f);

	tempMat = glm::rotate(tempMat, glm::radians(m_yaw), glm::vec3(0, 1, 0));
	tempMat = glm::rotate(tempMat, glm::radians(m_pitch), glm::vec3(1, 0, 0));

	m_viewMatrix = glm::inverse(tempMat);
}