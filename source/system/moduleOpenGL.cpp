#include "include/moduleOpenGL.h"

#include <common/include/common.h>
#include <fstream>
#include <iostream>
#include <thirdparty/include/thirdparty.h>

namespace Vision 
{ 
namespace System
{
//********************************
//     Class GraphicData
//********************************
//----------------------------------------------------------------
GraphicData::GraphicData(Core::Allocator& allocator)
        : mAllocator(allocator)
        , mVertices(nullptr)
        , mIndices(nullptr)
        , mMatrixTransform(nullptr)
{}

//----------------------------------------------------------------
GraphicData::GraphicData(Core::Allocator& allocator, std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, const Types::MatrixTransform transform /*= TransformMatrix(1.0f)*/, const size_t drawMode /*= GL_TRIANGLES*/)
    : mAllocator(allocator)
{
    mMatrixTransform = mAllocator.AllocateAndInsert(transform);
    mVertices = mAllocator.AllocateAndInsert(vertices);
    mIndices = mAllocator.AllocateAndInsert(indices);

    mGraphicInfo = mAllocator.Allocate<GraphicInfo>(1);
    GenerateGraphicInfo(vertices.size(), indices.size(), drawMode);    
}

//----------------------------------------------------------------
void GraphicData::SetBuffers(const GLuint vertexBuffer, const GLuint elementBuffer) const
{
    // feed Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mGraphicInfo->vertexCount * Types::sVertexElementInBytes, mVertices, mGraphicInfo->drawMode);
    // feed Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mGraphicInfo->indexCount * Types::sIndexElementBytes, mIndices, mGraphicInfo->drawMode);
}

//----------------------------------------------------------------
void GraphicData::GenerateGraphicInfo(const size_t vertexCount, const size_t indexCount, const size_t drawMode)
{
    assert(mGraphicInfo != nullptr);

    mGraphicInfo->vertexCount = vertexCount;
    mGraphicInfo->indexCount = indexCount;
    mGraphicInfo->drawMode = drawMode;
}

//********************************
//     Class Program
//********************************
//----------------------------------------------------------------
Program::Program(const char* vertexPath, const char* fragmentPath)
{
    Util::FileString code(vertexPath);
    GLuint vertexID = CompileShader(code.c_str(), GL_VERTEX_SHADER);
    code.ChangeFile(fragmentPath);
    GLuint fragmentID = CompileShader(code.c_str(), GL_FRAGMENT_SHADER);

    LinkProgram(vertexID, fragmentID);
    CleanShaders(vertexID, fragmentID);

    glEnable(GL_DEPTH_TEST);

    GenerateBuffers();
}

//----------------------------------------------------------------
const GLuint Program::CompileShader(const char* code, const GLuint type)
{
    GLuint id = glCreateShader(type);
    glShaderSource(id, 1, &code, NULL);
    glCompileShader(id);
    CheckErrors(id, true);
    return id;
}

//----------------------------------------------------------------
void Program::LinkProgram(const GLuint vertexID, const GLuint fragmentID)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);
    CheckErrors(ID, false);
}

//----------------------------------------------------------------
void Program::CheckErrors(GLuint shader, const bool isShader)
{
    GLint success;
    GLchar infoLog[1024];
    if (isShader)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LOG_STDERR(" Shader compilation error: " << infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG_STDERR("Linking error: " << infoLog);
        }
    }
}

//----------------------------------------------------------------
void Program::CleanShaders(const GLuint vertexID, const GLuint fragmentID)
{
    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);
}

//----------------------------------------------------------------
void Program::GenerateBuffers()
{
    glGenVertexArrays(1, &mVertexArrayObject);
    glGenBuffers(1, &mVertexBufferObject);
    glGenBuffers(1, &mElementArrayBuffer);

    glBindVertexArray(mVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, AttribArrayPtr::sStrideSize, (void*)AttribArrayPtr::sPointPointer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, AttribArrayPtr::sStrideSize, (void*)AttribArrayPtr::sColorPointer);
    glEnableVertexAttribArray(1);
}

//----------------------------------------------------------------
void Program::Use() const
{
    glUseProgram(ID);
}

//----------------------------------------------------------------
void Program::Draw(GraphicData& graphicData)
{
    glViewport(0, -100, 800, 800);
    glClearColor(0.3f, 0.0f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Use();

    graphicData.SetBuffers(mVertexBufferObject, mElementArrayBuffer);
    SetMatrix4f("model", graphicData.GetMatrixTransformVPtr());

    glBindVertexArray(mVertexArrayObject); 
    LOG_STDOUT(graphicData.GetIndexCount());
    glDrawElements(GL_TRIANGLES, graphicData.GetIndexCount(), GL_UNSIGNED_INT, 0);
}

void Program::SetMatrix4f(const char* name, GLfloat* value_ptr)
{
    const GLuint location = glGetUniformLocation(ID, name);
    assert(location != -1);

    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value_ptr);
}

} // namespace System
} // namespace Vision