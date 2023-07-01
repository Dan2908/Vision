#include <common/include/common.h>
#include <system/include/moduleSDL.h>
#include <system/include/moduleOpenGL.h>
#include <system/include/types.h>
#include <graphic.h>
#include <scenario.h>

namespace Vision 
{
namespace Testing
{
namespace ShapeConst
{
	using VertexT = System::Types::Float;
	using IndexT = System::Types::UInt;

	static const VertexT cubeVertex[] =
		{							  
/*				X      Y      Z     R     G     B   TxX	  TxY    */
			-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,  // front left  top
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,  // front right top
			-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	// front left  bottom
			 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  // front right bottom
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	// back  left  top
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,	// back  right top
			-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,	// back  left  bottom
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,	// back  right bottom
		
		};

#define CUBEIX(offset)\
		0 + offset, 1 + offset, 2 + offset, 1 + offset, 2 + offset, 3 + offset,\
		4 + offset, 5 + offset, 6 + offset, 5 + offset, 6 + offset, 7 + offset,\
		4 + offset, 5 + offset, 0 + offset, 5 + offset, 0 + offset, 1 + offset,\
		2 + offset, 3 + offset, 6 + offset, 3 + offset, 6 + offset, 7 + offset,\
		4 + offset, 0 + offset, 6 + offset, 0 + offset, 6 + offset, 2 + offset,\
		1 + offset, 5 + offset, 3 + offset, 5 + offset, 3 + offset, 7 + offset

	static const IndexT cubeIndex[] =
		{
			CUBEIX(0)
		};

	constexpr static const char* sTexGrassPath = "texture\\grassPattern.jpg";

	static Graphic::GraphicData sCube(std::initializer_list<VertexT>(cubeVertex, &cubeVertex[Util::RawArraySize(cubeVertex)]),
									  std::initializer_list<IndexT>(cubeIndex, &cubeIndex[Util::RawArraySize(cubeIndex)]),
									  { sTexGrassPath });
} //namespace ShapeConst

class TestInstance
{
	static TestInstance mInstance;
	
	using Programs = std::vector<System::Program*>;

	Programs mPrograms;
	System::Window* mWindow;
	Scenario::Scenario mScenario;

	glm::mat4 mModel = glm::identity<glm::mat4>();
	glm::mat4 mProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 700.0f);

	TestInstance() : mScenario()
	{}

	inline void NewProgram(const char* vPath, const char* fPath) { mInstance.mPrograms.push_back(new System::Program(vPath, fPath)); }
public:
	static void Initialize()
	{
		System::System::Start();

		mInstance.mWindow = new System::Window(800, 600, "Vision - Debug - Test Window");
		
		mInstance.NewProgram("shaders\\default_vs.glsl", "shaders\\default_fs.glsl");
	}

	static void Terminate()
	{
		mInstance.mWindow->~Window();
		System::System::Quit();
	}

	static const bool Load()
	{}

	static const int Run()
	{
		Initialize();

		System::Program& defaultShader = *mInstance.mPrograms.at(0);

		Scenario::Object cube(ShapeConst::sCube);
		mInstance.mScenario.LoadObject(cube);

		defaultShader.Use();
		defaultShader.LoadAllTexturesToGL();

		System::Event& event = System::EventManager::PollEvent();

		bool quit = false;
		bool refresh = true;

		Scenario::Camera& camera = mInstance.mScenario.GetCurrentCamera();
		while (!quit)
		{
			if (refresh)
			{
				mInstance.mScenario.SetAllBuffers(defaultShader.GetVertexBufferID(), defaultShader.GetElementArrayBufferID());

				defaultShader.SetMatrix4f("view", mInstance.mScenario.GetCurrentCameraView());
				defaultShader.SetMatrix4f("projection", mInstance.mProjection);
				defaultShader.SetMatrix4f("model", mInstance.mModel);
				defaultShader.Draw(mInstance.mScenario.GetDrawingInfo());

				mInstance.mWindow->Swap();

				mInstance.mScenario.GetCurrentCamera();
			}
			refresh = true;

			switch (event.GetAction())
			{
			case System::eActions::QUIT:
				quit = true;
				break;

			case System::eActions::LEFT:
				camera.RotateAround(System::Types::Vector3(0.0f, 0.01f, 0.0f));
				break;

			case System::eActions::RIGHT:
				camera.RotateAround(System::Types::Vector3(0.0f, -0.01f, 0.0f));
				break;

			case System::eActions::UP:
				camera.RotateAround(System::Types::Vector3(0.01f, 0.0f, 0.0f));
				break;

			case System::eActions::DOWN:
				camera.RotateAround(System::Types::Vector3(-0.01f, 0.0f, 0.0f));
				break;

			case System::eActions::PLUS:
				camera.ZoomIn();
				break;

			case System::eActions::MINUS:
				camera.ZoomOut();
				break;

			default:
				refresh = false;
			}

			System::EventManager::PollEvent();
		}

		Terminate();
		return 0;
	}
};

TestInstance TestInstance::mInstance;

const int Run()
{
	return TestInstance::Run();
}

} //namespace Testing
} //namespace Vision