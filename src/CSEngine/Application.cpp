#include "Application.h"
#include "ShaderProgram.h"
#include "ParticlesVA.h"
#include "Exception.h"

#include <exception>
#include <iostream>
#include <glm/ext.hpp>

Application::~Application()
{
	SDL_DestroyWindow(m_window);
}

std::shared_ptr<Application> const Application::Init()
{
	std::shared_ptr<Application> app = std::make_shared<Application>();

	if (SDL_Init(SDL_INIT_VIDEO) < 0) { throw std::exception(); }
	app->m_window = SDL_CreateWindow("my game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	if (!SDL_GL_CreateContext(app->m_window)) { throw std::exception(); }
	if (glewInit() != GLEW_OK) { throw std::exception(); }

	return app;
}

void Application::Run()
{
	m_running = true;
	SDL_Event event;

	m_computeShader = std::make_shared<ShaderProgram>();
	m_renderingShader = std::make_shared<ShaderProgram>();
	m_particles = std::make_shared<ParticlesVA>();

	//m_computeShader->CreateComputeShader("/path");
	m_renderingShader->CreateShader("../src/resources/shaders/particleShader.vert", 
									"../src/resources/shaders/particleShader.frag");
	m_particles->Init();
	std::cout << "particles initialised" << std::endl;
	while (m_running)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: m_running = false; break;
			}
		}
		//Set the projection matrix in the shader
		m_renderingShader->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
			(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 230.f));

		m_renderingShader->Draw(m_particles);

		SDL_GL_SwapWindow(m_window);
	}
}