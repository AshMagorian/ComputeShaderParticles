#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <memory>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

class ShaderProgram;
class ParticlesVA;
class Camera;

class Application
{
private:
	SDL_Window* m_window;
	bool m_running;

	int m_deltaMouseX;
	int m_deltaMouseY;

	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<ShaderProgram> m_computeShader;
	std::shared_ptr<ShaderProgram> m_renderingShader;
	std::shared_ptr<ParticlesVA> m_particles;
public:
	Application() {}
	~Application();
	static std::shared_ptr<Application> const Init();

	void Run();
};

#endif