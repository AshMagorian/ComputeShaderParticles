#include "ShaderProgram.h"
#include "Exception.h"
#include "ParticlesVA.h"
#include "Texture.h"

#include <iostream>
#include <glm/ext.hpp>
#include <vector>

ShaderProgram::ShaderProgram()
{
	id = glCreateProgram();
}

void ShaderProgram::CreateShader(std::string _vert, std::string _frag)
{
	GLuint vertexShaderId = AttachVetexShader(_vert);
	GLuint fragmentShaderId = AttachFragmentShader(_frag);

	glBindAttribLocation(id, 0, "in_Position");
	glBindAttribLocation(id, 1, "in_Color");
	glBindAttribLocation(id, 2, "in_TexCoord");

	// Perform the link and check for faliure
	glLinkProgram(id);
	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw Exception("Shader cannot be created");
	}
	glUseProgram(id);

	glDetachShader(id, vertexShaderId);
	glDeleteShader(vertexShaderId);
	glDetachShader(id, fragmentShaderId);
	glDeleteShader(fragmentShaderId);
}

void ShaderProgram::CreateComputeShader(std::string _comp)
{
	GLuint computeShaderId = AttachComputeShader(_comp);

	// Perform the link and check for faliure
	glLinkProgram(id);
	GLint success = 0;
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		throw Exception("Shader cannot be created");
	}
	glUseProgram(id);
	glDetachShader(id, computeShaderId);
	glDeleteShader(computeShaderId);
}

GLuint ShaderProgram::AttachVetexShader(std::string _path)
{
	std::string vertShader;
	std::ifstream file(_path);
	if (!file.is_open())
	{
		throw Exception("Vertex shader not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			vertShader += line + "\n";
		}
	}
	file.close();

	const char *vertex = vertShader.c_str();
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertex, NULL);
	glCompileShader(vertexShaderId);
	GLint success = 0;
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(vertexShaderId, maxLength, &maxLength, &errorlog[0]);
		std::cout << &errorlog.at(0) << std::endl;
		throw Exception("Vertex shader compile error: " + (std::string)&errorlog.at(0));
	}
	glAttachShader(id, vertexShaderId);
	return vertexShaderId;
}

GLuint ShaderProgram::AttachFragmentShader(std::string _path)
{
	std::string fragShader;
	std::ifstream file(_path);

	if (!file.is_open())
	{
		throw Exception("Fragment shader not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			fragShader += line + "\n";
		}
	}
	file.close();

	const char *fragment = fragShader.c_str();
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragment, NULL);
	glCompileShader(fragmentShaderId);
	GLint success = 0;
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(fragmentShaderId, maxLength, &maxLength, &errorlog[0]);
		std::cout << &errorlog.at(0) << std::endl;
		throw Exception("Fragment shader compile error: " + (std::string)&errorlog.at(0));
	}
	glAttachShader(id, fragmentShaderId);
	return fragmentShaderId;
}

GLuint ShaderProgram::AttachComputeShader(std::string _path)
{
	std::string computeShader;
	std::ifstream file(_path);

	if (!file.is_open())
	{
		throw Exception("Compute shader not found, '" + _path + "' cannot be loaded");
	}
	else
	{
		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);
			computeShader += line + "\n";
		}
	}
	file.close();

	const char *compute = computeShader.c_str();
	GLuint computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(computeShaderId, 1, &compute, NULL);
	glCompileShader(computeShaderId);
	GLint success = 0;
	glGetShaderiv(computeShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint maxLength = 0;
		glGetShaderiv(computeShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorlog(maxLength);
		glGetShaderInfoLog(computeShaderId, maxLength, &maxLength, &errorlog[0]);
		std::cout << &errorlog.at(0) << std::endl;
		throw Exception("Compute shader compile error: " + (std::string)&errorlog.at(0));
	}
	m_isComputeShader = true;
	glAttachShader(id, computeShaderId);
	return computeShaderId;
}

void ShaderProgram::Draw(std::shared_ptr<ParticlesVA> _va)
{
	glUseProgram(id);
	glBindVertexArray(_va->GetId());
	
	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		if (samplers.at(i).texture)
		{
			glBindTexture(GL_TEXTURE_2D, samplers.at(i).texture->getId());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _va->GetNumParticles());

	//Unbinds the textures for all samplers
	for (size_t i = 0; i < samplers.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindVertexArray(0);
	glUseProgram(0);
}

void ShaderProgram::InvokeComputeShader(std::shared_ptr<ParticlesVA> _va)
{
	if (m_isComputeShader == true)
	{
		glUseProgram(id);
		glDispatchCompute(_va->GetNumParticles() / _va->GetWorkGroupSize(), 1, 1);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	}
}

void ShaderProgram::SetUniform(std::string uniform, glm::vec3 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform3f(uniformId, value.x, value.y, value.z);
	glUseProgram(0);
}
void ShaderProgram::SetUniform(std::string uniform, glm::vec4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniform4f(uniformId, value.x, value.y, value.z, value.w);
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, glm::mat4 value)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}

	glUseProgram(id);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, glm::value_ptr(value));
	glUseProgram(0);
}

void ShaderProgram::SetUniform(std::string uniform, std::shared_ptr<Texture> texture)
{
	GLint uniformId = glGetUniformLocation(id, uniform.c_str());

	if (uniformId == -1)
	{
		throw std::exception();
	}
	//Tries to find the correct sampler in the vector
	for (size_t i = 0; i < samplers.size(); i++)
	{
		if (samplers.at(i).id == uniformId)
		{
			samplers.at(i).texture = texture;

			glUseProgram(id);
			glUniform1i(uniformId, i);
			glUseProgram(0);
			return;
		}
	}
	//If the sampler isn't found then a new sampler is made and pushed back
	Sampler s;
	s.id = uniformId;
	s.texture = texture;
	samplers.push_back(s);

	glUseProgram(id);
	glUniform1i(uniformId, samplers.size() - 1);
	glUseProgram(0);
}