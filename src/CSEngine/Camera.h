#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
private:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
public:
	Camera() {}
	~Camera() {}

	void UpdateMatrix(int _dx, int _dy);

	glm::mat4 GetViewMatrix() { return m_viewMatrix; }
	glm::mat4 GetProjectionMatrix() { return m_projectionMatrix; }

};
#endif