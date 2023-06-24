#include <system/include/types.h>
#include <common/include/common.h>
#

	
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

	static const IndexT cubeIndex[] =
		{
			0, 1, 2, 1, 2, 3, //Front
			4, 5, 6, 5, 6, 7, //Back
			4, 5, 0, 5, 0, 1, //Top
			2, 3, 6, 3, 6, 7, //Bottom
			4, 0, 6, 0, 6, 2, //Left
			1, 5, 3, 5, 3, 7  //Right
		};

	constexpr static const char* sTexGrassPath = "texture\\grassPattern.jpg";

	System::GraphicData* MakeCube()
	{
		System::GraphicData* output = new System::GraphicData(
			std::initializer_list<VertexT>(cubeVertex, &cubeVertex[Util::RawArraySize(cubeVertex)]),
			std::initializer_list<IndexT>(cubeIndex, &cubeIndex[Util::RawArraySize(cubeIndex)]),
			{ sTexGrassPath }
		);

		return output;
	}
} //namespace ShapeConst

class TestInstance
{
	static TestInstance mInstance;
	
	System::Window* mWindow;
	System::Program* mProgram;
	System::Camera mCamera;

	glm::mat4 mModel = glm::identity<glm::mat4>();
	glm::mat4 mProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 700.0f);

	std::vector<System::GraphicData*> mGraphicObjs;

	TestInstance() : mCamera(), mGraphicObjs()
	{}
public:
	static void Initialize()
	{
		System::System::Start();

		mInstance.mWindow = new System::Window(800, 600, "Vision - Debug - Test Window");
		mInstance.mProgram = new System::Program("shaders\\default_vs.glsl", "shaders\\default_fs.glsl");
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

		mInstance.mGraphicObjs.push_back(ShapeConst::MakeCube());

		mInstance.mProgram->Use();
		mInstance.mProgram->LoadAllTexturesToGL();

		System::Event& event = System::EventManager::PollEvent();

		bool quit = false;
		bool refresh = true;

		while (!quit)
		{
			if (refresh)
			{
				mInstance.mProgram->SetMatrix4f("view", mInstance.mCamera.GetView());
				mInstance.mProgram->SetMatrix4f("projection", mInstance.mProjection);
				mInstance.mProgram->SetMatrix4f("model", mInstance.mModel);

				mInstance.mProgram->Draw(*mInstance.mGraphicObjs.at(0));
				mInstance.mWindow->Swap();
			}
			refresh = true;

			switch (event.GetAction())
			{
			case System::eActions::QUIT:
				quit = true;
				break;

			case System::eActions::LEFT:
				mInstance.mCamera.RotateAround(System::Types::Vector3(0.0f, 0.01f, 0.0f));
				break;

			case System::eActions::RIGHT:
				mInstance.mCamera.RotateAround(System::Types::Vector3(0.0f, -0.01f, 0.0f));
				break;

			case System::eActions::UP:
				mInstance.mCamera.RotateAround(System::Types::Vector3(0.01f, 0.0f, 0.0f));
				break;

			case System::eActions::DOWN:
				mInstance.mCamera.RotateAround(System::Types::Vector3(-0.01f, 0.0f, 0.0f));
				break;

			case System::eActions::PLUS:
				mInstance.mCamera.ZoomIn();
				break;

			case System::eActions::MINUS:
				mInstance.mCamera.ZoomOut();
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