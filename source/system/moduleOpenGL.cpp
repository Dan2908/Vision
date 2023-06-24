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
GraphicData::GraphicData(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, std::initializer_list<const char*> texturePaths)
    : mVertices(vertices)
    , mIndices(indices)
    , mTextures()
    , mMatrixTransform(1.0f)
{
    if (texturePaths.size() > 0)
    {
        for (const char* path : texturePaths)
        {
            mTextures.push_back(&TextureLoader::AddTexture(path));
        }
    }
}

//----------------------------------------------------------------
void GraphicData::SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer)
{
    // feed Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * Types::VertexConst::sVertexElementInBytes, mVertices.data(), GL_DYNAMIC_DRAW);
    // feed Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * Types::VertexConst::sIndexElementBytes, mIndices.data(), GL_STATIC_DRAW);

    const size_t nTextures = mTextures.size();

    assert(nTextures < 32);

    for (int i = 0; i < nTextures; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mTextures.at(i)->id);
    }
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
void Program::Draw(GraphicData& graphicData)
{
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    graphicData.SetBuffers(mVertexArrayBuffer, mElementArrayBuffer);

    glBindVertexArray(mVertexArrayObject);
    glDrawElements(GL_TRIANGLES, graphicData.GetIndexCount(), GL_UNSIGNED_INT, 0);
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
    TextureMap& texList = TextureLoader::GetTextureList();
    for (auto& tex : texList)
    {
        LoadTextureToGL(tex.second);
    }
}

//********************************
//     Class TextureLoader
//********************************

TextureLoader* TextureLoader::mInstance = new TextureLoader();
//----------------------------------------------------------------
TextureLoader::TextureLoader()
    : mTextureList()
{
}

//----------------------------------------------------------------
Types::TextureInfo& TextureLoader::iAddTexture(const char* path, const char* name /*= "unnamed"*/)
{
    // No repeated names allowed, automatically renamed at this point.
    std::string rename(name);
    int renameIndex = 1;
    TextureMap& texList = mInstance->mTextureList;

    while (texList.find(rename) != texList.end())
    {
        rename.assign(name);
        rename.append(std::to_string(renameIndex++));
    }
    // emplace
    texList[rename] = *new Types::TextureInfo(path);

    return texList[rename];
}


//----------------------------------------------------------------
const bool TextureLoader::iRemoveTexture(const char* name)
{
    TextureMap& texList = mInstance->mTextureList;

    if (texList.find(name) != texList.end())
    {
        texList.erase(name);
        return true;
    }
    return false;
}

//----------------------------------------------------------------
const GLuint TextureLoader::iGetTexture(const char* name)
{
    auto tex = mInstance->mTextureList.find(name);
    if (tex != mInstance->mTextureList.end())
    {
        return tex->second.id;
    }

    return -1;
}

//----------------------------------------------------------------
TextureMap& TextureLoader::iGetTextureList()
{
    return mInstance->mTextureList;
}

//********************************
//     Class Camera
//********************************
//----------------------------------------------------------------
Camera::Camera()
    : mPosition(0.0f, 0.0f, 10.0f)
    , mView()
{
    mView = glm::lookAt(mPosition, Types::Vector3(0.0f), Types::VECTOR_UP);
}

//----------------------------------------------------------------
void Camera::Move(const Types::Vector3& direction)
{
    mPosition += direction;
    mView = glm::lookAt(mPosition, Types::Vector3(0.0f), Types::VECTOR_UP);
}

//----------------------------------------------------------------
const Types::Matrix44& Camera::GetView() const
{
    return mView;
}

} // namespace System
} // namespace Vision