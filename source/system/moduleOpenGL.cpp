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
GraphicData::GraphicData(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices)
    : mVertices(vertices)
    , mIndices(indices)
    , mMatrixTransform(1.0f)
{}

//----------------------------------------------------------------
void GraphicData::SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer)
{
    // feed Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * Types::sVertexElementInBytes, mVertices.data(), GL_STATIC_DRAW);
    // feed Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * Types::sIndexElementBytes, mIndices.data(), GL_STATIC_DRAW);
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
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexArrayBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexArrayBuffer);

    glGenBuffers(1, &mElementArrayBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementArrayBuffer);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, AttribArrayPtr::sStrideSize, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, AttribArrayPtr::sStrideSize, (void*)(3*sizeof(GLfloat)));
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
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    graphicData.SetBuffers(mVertexArrayBuffer, mElementArrayBuffer);

    glBindVertexArray(mVertexArrayObject);
    glDrawElements(GL_TRIANGLES, graphicData.GetIndexCount(), GL_UNSIGNED_INT, 0);
}

void Program::SetMatrix4f(const char* name, const Types::MatrixTransform& matrix)
{
    const GLuint location = glGetUniformLocation(ID, name);
    assert(location != -1);

    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

//********************************
//     Class Camera
//********************************
//----------------------------------------------------------------
Camera::Camera()
    : mPosition(0.0f, 0.0f, 3.0f)
    , mView()
{
    mView = glm::lookAt(mPosition, Types::Vector(0.0f), Types::VECTOR_UP);
}

//----------------------------------------------------------------
void Camera::Move(const Types::Vector& direction)
{
    mPosition += direction;
    mView = glm::lookAt(mPosition, Types::Vector(0.0f), Types::VECTOR_UP);
}

//----------------------------------------------------------------
const Types::MatrixTransform& Camera::GetView() const
{
    return mView;
}

} // namespace System
} // namespace Vision