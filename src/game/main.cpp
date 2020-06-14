#include <CSEngine/CSEngine.h>


int main(int argc, char *argv[])
{
	std::shared_ptr<Application> application = Application::Init();

	application->Run();

	return 0;
}
