#include <system/include/moduleSDL.h>
#include <system/include/moduleOpenGL.h>

using namespace Vision;

int main(int arc, char* argv[]) 
{
	System::System::Start();
	System::Window window(800, 600);

	while (System::EventManager::PollEvent().GetAction() != System::eActions::QUIT)
	{}

	window.~Window();
	System::System::Quit();
	return 0;
}