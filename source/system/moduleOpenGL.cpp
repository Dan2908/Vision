#include "include/moduleOpenGL.h"

#include <common/include/common.h>
#include <fstream>
#include <graphic.h>
#include <iostream>
#include <thirdparty/include/thirdparty.h>

namespace Vision 
{ 
namespace System
{

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

    glVertexAttribPointer(0, 3, GL_FLOAT, false, Types::VertexConst::sStrideSize, (void*)Types::VertexConst::sPointPointer);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, Types::VertexConst::sStrideSize, (void*)Types::VertexConst::sColorPointer);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, Types::VertexConst::sStrideSize, (void*)Types::VertexConst::sTexturePointer);
    glEnableVertexAttribArray(2);
}

//----------------------------------------------------------------
void Program::Use() const
{
    glUseProgram(ID);
}

//----------------------------------------------------------------
void Program::Draw(const DrawingInfo drawingInfo)
{
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //NOTE: Set the buffers in the scenario

    glBindVertexArray(mVertexArrayObject);
    glDrawElements(drawingInfo.drawType, drawingInfo.indexCount, GL_UNSIGNED_INT, 0);
}

//----------------------------------------------------------------
void Program::SetMatrix4f(const char* name, const Types::Matrix44& matrix)
{
    const GLuint location = glGetUniformLocation(ID, name);
    assert(location != -1);

    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

//----------------------------------------------------------------
const bool Program::LoadTextureToGL(Types::TextureInfo& texture)
{
    if (texture.CheckInfo())
    {
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
        glGenerateMipmap(GL_TEXTURE_2D);

        return true;
    }

    return false;
}

//----------------------------------------------------------------
void Program::LoadAllTexturesToGL()
{
    Graphic::TextureMap& texList = Graphic::TextureLoader::GetTextureList();
    for (auto& tex : texList)
    {
        LoadTextureToGL(tex.second);
    }
}

} // namespace System
} // namespace Vision