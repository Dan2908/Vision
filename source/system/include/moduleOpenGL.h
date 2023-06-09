#pragma once

#include <core/include/vmemory.h>
#include <thirdparty.h>
#include <map>
#include <vector>
#include <system/include/types.h>
#include <fileManager.h>
#include <graphic/include/graphic.h>

namespace Vision
{
namespace System
{
enum eDrawType
{
    TRIANGLES = GL_TRIANGLES
};

struct DrawingInfo
{
    eDrawType drawType = eDrawType::TRIANGLES;
    Types::UInt indexCount = 0;
};

class Program
{
    GLuint mVertexArrayObject;
    GLuint mVertexArrayBuffer;
    GLuint mElementArrayBuffer;
     
    const GLuint CompileShader(const char* code, const GLuint type);
    void LinkProgram(const GLuint vertexID, const GLuint fragmentID);
    void CheckErrors(GLuint shader, const bool isShader);
    void CleanShaders(const GLuint vertexID, const GLuint fragmentID);
    void GenerateBuffers();

public:
    GLuint ID;

    inline Types::UInt& GetVertexBufferID() { return mVertexArrayBuffer;  }
    inline Types::UInt& GetElementArrayBufferID() { return mElementArrayBuffer;  }

    Program() {}
    Program(const char* vertexPath, const char* fragmentPath);
    void Use() const;
    void SetMatrix4f(const char* name, const Types::Matrix44& matrix);
    const bool LoadTextureToGL(Types::TextureInfo& texture);
    void LoadAllTexturesToGL();
    void Draw(const DrawingInfo drawingInfo);
};

namespace GL
{
    const bool CheckCompileStatus(Types::UInt shader);
    const bool CheckLinkStatus(Types::UInt program);
    const Types::UInt CompileShader(const char* code, const Types::UInt type);
    // Creates shader program. NOTE: shaders are deleted after linking
    void CreateProgram(Types::UInt programID, const Types::UInt vertexShaderID, const Types::UInt fragmentShaderID);
} // namespace GL

} // namespace System 
} // namespace Vision