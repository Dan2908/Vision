
#define STB_IMAGE_IMPLEMENTATION

#include <thirdparty.h>
#include <system/include/moduleSDL.h>
#include <system/include/moduleOpenGL.h>
#include <core/include/vmemory.h>

using namespace Vision;


System::GraphicData Cube(
{/*	   X      Y      Z     R     G     B   */

	-0.5f,  0.5f,  0.5f, 0.5f, 0.7f, 0.3f, // front left  top
	 0.5f,  0.5f,  0.5f, 0.8f, 1.0f, 0.6f, // front right top
	-0.5f, -0.5f,  0.5f, 0.5f, 0.7f, 0.3f, // front left  bottom
	 0.5f, -0.5f,  0.5f, 0.8f, 1.0f, 0.6f, // front right bottom
	-0.5f,  0.5f, -0.5f, 0.8f, 1.0f, 0.6f, // back  left  top
	 0.5f,  0.5f, -0.5f, 0.8f, 1.0f, 0.6f, // back  right top
	-0.5f, -0.5f, -0.5f, 0.8f, 1.0f, 0.6f, // back  left  bottom
	 0.5f, -0.5f, -0.5f, 0.8f, 1.0f, 0.6f, // back  right bottom
},
{
	//Front
	0, 1, 2,
	1, 2, 3,
	//Back
	4, 5, 6,
	5, 6, 7,
	//Top
	4, 5, 0,
	5, 0, 1,
	//Bottom
	2, 3, 6,
	3, 6, 7,
	//Left
	4, 0, 6,
	0, 6, 2,
	//Right
	1, 5, 3,
	5, 3, 7

});

int main(int arc, char* argv[]) 
{
	System::System::Start();
	System::Window window(800, 600);

	System::Program program("shaders\\default_vs.glsl", "shaders\\default_fs.glsl");
	program.Use();

	glm::mat4 model = glm::identity<glm::mat4>();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 700.0f);

	System::Camera camera;

	System::Event& event = System::EventManager::PollEvent();

	bool quit = false;
	bool refresh = true;
	while (!quit)
	{
		if (refresh)
		{
			program.SetMatrix4f("view", camera.GetView());
			program.SetMatrix4f("projection", projection);
			program.SetMatrix4f("model", model);

			program.Draw(Cube);
			window.Swap();
		}

		refresh = true;

		switch (event.GetAction())
		{
		case System::eActions::QUIT:
			quit = true;
			break;

		case System::eActions::LEFT:
			camera.Move(System::Types::Vector(0.1f, 0.0f, 0.0f));
			break;

		case System::eActions::RIGHT:
			camera.Move(System::Types::Vector(-0.1f, 0.0f, 0.0f));
			break;

		case System::eActions::UP:
			camera.Move(System::Types::Vector(0.0f, 0.1f, 0.0f));
			break;

		case System::eActions::DOWN:
			camera.Move(System::Types::Vector(0.0f, -0.1f, 0.0f));
			break;
		default:
			refresh = false;
		}

		System::EventManager::PollEvent();
	}

	window.~Window();
	System::System::Quit();

	return 0;
}
