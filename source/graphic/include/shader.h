#pragma once

#include <system/include/moduleOpenGL.h>


namespace Vision
{
namespace Graphic
{

static const char* sDefaultVShaderPath = "shaders\\default_vs.glsl";
static const char* sDefaultFShaderPath = "shaders\\default_fs.glsl";

class Shader
{
protected:
	System::Types::UInt mId;
	std::vector<System::Types::UInt> mBuffers;

	void Initialize(const char* vertexPath, const char* fragmentPath);

public:
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);

	GETSET_MEMBER(System::Types::UInt, Id);
};

class DefaultShader : public Shader
{
public:
	DefaultShader()
		: Shader(sDefaultVShaderPath, sDefaultFShaderPath)
	{}

	GETSET_ARRAY_AT(System::Types::UInt, mBuffers, 0, VAOId);
	GETSET_ARRAY_AT(System::Types::UInt, mBuffers, 1, VertexBufferId);
	GETSET_ARRAY_AT(System::Types::UInt, mBuffers, 2, ElementBufferId);
};

} // namespace Graphic
} // namespace Vision