#include <graphic/include/shader.h>

#include <common/include/common.h>

namespace Vision
{
namespace Graphic
{

//********************************
//     Class Shader
//********************************
//----------------------------------------------------------------
Shader::Shader()
	: mId(0)
	, mBuffers()
{}

//----------------------------------------------------------------
Shader::Shader(const char* vertexPath, const char* fragmentPath)
	: Shader()
{
	Initialize(vertexPath, fragmentPath);
}

//----------------------------------------------------------------
void Shader::Initialize(const char* vertexPath, const char* fragmentPath)
{
	// NOTE: check if FileString survives to provide the code
	System::Types::UInt vertexShader = System::GL::CompileShader(Util::FileString(vertexPath).c_str(), GL_VERTEX_SHADER);
	System::Types::UInt fragmentShader = System::GL::CompileShader(Util::FileString(fragmentPath).c_str(), GL_FRAGMENT_SHADER);

	System::GL::CreateProgram(mId, vertexShader, fragmentShader);
}

} // namespace Graphic
} // namespace Vision