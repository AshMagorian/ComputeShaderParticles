#include "ParticlesVA.h"

#include <cstdlib>
#include <ctime>
#include <exception>

ParticlesVA::ParticlesVA()
{
	glGenVertexArrays(1, &id);
	if (!id)
	{
		throw std::exception();
	}
}

void ParticlesVA::Init()
{
	glGenBuffers(1, &posSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numParticles * sizeof(struct pos), NULL, GL_STATIC_DRAW);

	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing

	m_points = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_numParticles * sizeof(struct pos), bufMask);
	for (int i = 0; i < m_numParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_points[i].x = (rndm*(m_xMax - m_xMin)) + m_xMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_points[i].y = (rndm*(m_yMax - m_yMin)) + m_yMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_points[i].z = (rndm*(m_zMax - m_zMin)) + m_zMin;
		m_points[i].w = 1.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);


	glGenBuffers(1, &velSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numParticles * sizeof(struct vel), NULL, GL_STATIC_DRAW);
	m_vels = (struct vel *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_numParticles * sizeof(struct vel), bufMask);
	for (int i = 0; i < m_numParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_vels[i].vx = (rndm*(m_vxMax - m_vxMin)) + m_vxMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_vels[i].vy = (rndm*(m_vyMax - m_vyMin)) + m_vyMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_vels[i].vz = (rndm*(m_vzMax - m_vzMin)) + m_vzMin;
		m_vels[i].vw = 0.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	glGenBuffers(1, &colSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, colSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numParticles * sizeof(struct color), NULL, GL_STATIC_DRAW);
	m_colors = (struct color *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_numParticles * sizeof(struct color), bufMask);
	for (int i = 0; i < m_numParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_colors[i].r = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_colors[i].g = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		m_colors[i].b = rndm;
		m_colors[i].a = 1.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

GLuint ParticlesVA::GetId()
{
	glBindVertexArray(id);

	glBindBuffer(GL_ARRAY_BUFFER, posSSbo);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(
		4, // attribute
		4, //size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, colSSbo);
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(
		6, // attribute
		4, //size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return id;
}