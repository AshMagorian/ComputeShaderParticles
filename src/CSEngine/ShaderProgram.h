#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <vector>

class ParticlesVA;
class Texture;

struct Sampler
{
	GLint id;
	std::shared_ptr<Texture> texture;
};

class ShaderProgram
{
private:
	GLuint id;
	std::vector<Sampler> samplers;

	GLuint AttachVetexShader(std::string _path);
	GLuint AttachFragmentShader(std::string _path);
	GLuint AttachComputeShader(std::string _path);

	bool m_isComputeShader = false;

public:
	ShaderProgram();

	void CreateShader(std::string _vert, std::string _frag);
	void CreateComputeShader(std::string _comp);

	void Draw(std::shared_ptr<ParticlesVA> _va);
	void InvokeComputeShader(std::shared_ptr<ParticlesVA> _va);

	void SetUniform(std::string uniform, glm::vec3 value);
	void SetUniform(std::string uniform, glm::vec4 value);
	void SetUniform(std::string uniform, glm::mat4 value);
	void SetUniform(std::string uniform, std::shared_ptr<Texture> texture);
};

#endif