#include <system/include/moduleSDL.h>
#include <system/include/moduleOpenGL.h>
#include <core/include/vmemory.h>

using namespace Vision;

class Object
{
	Object* mParent;
	Core::Allocator* mAllocator;
public:
	Object(Core::Allocator* allocator, Object* parent)
		: mParent(parent)
		, mAllocator(allocator)
	{}
};

class Factory : Object
{
	static Factory mInstance;
	Factory() 
		: Object(new Core::Allocator(), nullptr) 
	{}
public:
	Factory(const Factory&) = delete; // Delete copy constructor
	Factory(Factory&&) = delete; // Delete move constructor

	Object* CreateObject()
	{

	}
};



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