#include "ParticlesVA.h"

#include <cstdlib>
#include <ctime>
#include <exception>
#include <vector>

ParticlesVA::ParticlesVA()
{
	glGenVertexArrays(1, &id);
	if (!id)
	{
		throw std::exception();
	}
}

void ParticlesVA::InitBuffers()
{
	glGenBuffers(1, &billboardvbo);
	glBindBuffer(GL_ARRAY_BUFFER, billboardvbo);
	static const GLfloat data[] = { -0.5f, -0.5f, 0.0f, // Top left 
							 0.5f, -0.5f, 0.0f,  // Top right
							 -0.5f, 0.5f, 0.0f,	 // Bottom left
							  0.5f, 0.5, 0.0f };  // Bottom right
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, billboardvbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		0, (void *)0);
	glVertexAttribDivisor(0, 0); // one per vertex
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &posSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numParticles * sizeof(struct pos), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &velSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numParticles * sizeof(struct vel), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &colSSbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, colSSbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, m_numParticles * sizeof(struct color), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticlesVA::InitParticles()
{
	GLint bufMask = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT; // the invalidate makes a big difference when re-writing

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, posSSbo);
	struct pos* points = (struct pos *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_numParticles * sizeof(struct pos), bufMask);
	for (int i = 0; i < m_numParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		points[i].x = (rndm*(m_xMax - m_xMin)) + m_xMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		points[i].y = (rndm*(m_yMax - m_yMin)) + m_yMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		points[i].z = (rndm*(m_zMax - m_zMin)) + m_zMin;
		points[i].w = 1.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, velSSbo);
	struct vel* vels = (struct vel *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_numParticles * sizeof(struct vel), bufMask);
	for (int i = 0; i < m_numParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		vels[i].vx = (rndm*(m_vxMax - m_vxMin)) + m_vxMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		vels[i].vy = (rndm*(m_vyMax - m_vyMin)) + m_vyMin;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		vels[i].vz = (rndm*(m_vzMax - m_vzMin)) + m_vzMin;
		vels[i].vw = 0.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, colSSbo);
	struct color* colors = (struct color *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_numParticles * sizeof(struct color), bufMask);
	for (int i = 0; i < m_numParticles; i++)
	{
		float rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		colors[i].r = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		colors[i].g = rndm;
		rndm = (static_cast<float> (rand()) / static_cast<float> (RAND_MAX));
		colors[i].b = rndm;
		colors[i].a = 1.0f;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, posSSbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, velSSbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, colSSbo);
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
	glVertexAttribDivisor(4, 1); // one per quad

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
	glVertexAttribDivisor(6, 1); // one per quad

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return id;
}