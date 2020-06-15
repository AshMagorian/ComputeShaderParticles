#include "CSEngine.h"

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
	glEnable(GL_DEPTH_TEST);
	return app;
}

void Application::Run()
{
	m_running = true;
	SDL_Event event;

	m_camera = std::make_shared<Camera>();
	m_computeShader = std::make_shared<ShaderProgram>();
	m_renderingShader = std::make_shared<ShaderProgram>();
	m_particles = std::make_shared<ParticlesVA>();

	m_computeShader->CreateComputeShader("../src/resources/shaders/particleShader.comp");
	m_renderingShader->CreateShader("../src/resources/shaders/particleShader.vert", 
									"../src/resources/shaders/particleShader.frag");
	m_particles->InitBuffers();
	m_particles->InitParticles();
	std::cout << "particles initialised" << std::endl;

	SDL_WarpMouseInWindow(m_window, int(WINDOW_WIDTH / 2.0f), int(WINDOW_HEIGHT / 2.0f)); // moves the mouse to the middle of the window
	SDL_ShowCursor(SDL_DISABLE); // Hides the cursor
	while (m_running)
	{
		/*
		* Process inputs
		*/
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: m_running = false; break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) { m_running = false; }
				if (event.key.keysym.sym == SDLK_1) { m_particles->InitParticles(); }
				break;
			}
			if (event.type == SDL_MOUSEMOTION) // Reads the mouse
			{
				int m_mouseX = event.motion.x;
				int m_mouseY = event.motion.y;
				m_deltaMouseX = int(WINDOW_WIDTH / 2.0f) - m_mouseX;  // deltaX = centre of screen - mouseX
				m_deltaMouseY = int(WINDOW_HEIGHT / 2.0f) - m_mouseY;	 // deltaY = centre of screen - mouseY
			}
		}

		SDL_WarpMouseInWindow(m_window, int(WINDOW_WIDTH / 2.0f), int(WINDOW_HEIGHT / 2.0f)); // moves the mouse to the middle of the window

		/*
		* Clears the screen
		*/
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_camera->UpdateMatrix(m_deltaMouseX, m_deltaMouseY);

		/*
		* Set the projection and view matrix in the shader
		*/
		m_renderingShader->SetUniform("in_Projection", glm::perspective(glm::radians(45.0f),
			(float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 230.f));
		m_renderingShader->SetUniform("in_View", m_camera->GetViewMatrix());

		m_computeShader->InvokeComputeShader(m_particles);
		m_renderingShader->Draw(m_particles);

		SDL_GL_SwapWindow(m_window);
	}
}