#ifndef PARTICLESA_VA
#define PARTICLES_VA

#include <GL/glew.h>
#include <SDL2/SDL.h>

struct pos
{
	float x, y, z, w; // positions
};
struct vel
{
	float vx, vy, vz, vw; // velocities
};
struct color
{
	float r, g, b, a; // colors
};

class ParticlesVA
{
private:
	GLuint id;

	int m_numParticles = 1024 * 1024;
	int m_workGroupSize = 128;

	float m_xMin = -10.0f;
	float m_xMax = 10.0f;
	float m_yMin = -10.0f;
	float m_yMax = 10.0f;
	float m_zMin = -10.0f;
	float m_zMax = 10.0f;

	float m_vxMin = -2.0f;
	float m_vxMax = 2.0f;
	float m_vyMin = -2.0f;
	float m_vyMax = 2.0f;
	float m_vzMin = -2.0f;
	float m_vzMax = 2.0f;

	GLuint posSSbo;
	GLuint velSSbo;
	GLuint colSSbo;

	struct pos* m_points;
	struct vel* m_vels;
	struct color* m_colors;

public:

	ParticlesVA();

	void Init();

	int GetNumParticles() { return m_numParticles; }
	int GetWorkGroupSize() { return m_workGroupSize; }

	GLuint GetId();
};
#endif