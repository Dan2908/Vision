#include "moduleOpenGL.h"

#include <common.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>

namespace GameDisplay 
{ 
namespace GL
{
//********************************
//     Class GraphicData
//********************************
//----------------------------------------------------------------
GraphicData::GraphicData(Memory::Allocator& allocator)
        : mAllocator(allocator)
        , mVertices(nullptr)
        , mIndices(nullptr)
        , mTransform(1.0f)
        , mVertexCount(0)
        , mIndexCount(0)
        , mDrawMode(GL_TRIANGLES)
{}

//----------------------------------------------------------------
GraphicData::GraphicData(Memory::Allocator& allocator, std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, const TransformMatrix transform /*= TransformMatrix(1.0f)*/, const size_t drawMode /*= GL_TRIANGLES*/)
        : mAllocator(allocator)
    {
        Load(vertices, indices, transform, drawMode);
    }

//----------------------------------------------------------------
void GraphicData::Load(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, const TransformMatrix transform, size_t drawMode = GL_TRIANGLES)
{
    assert((vertices.size() % AttribArrayPtr::Size()) == 0);

    mVertexCount = vertices.size();
    mIndexCount = indices.size();
    mTransform = transform;

    mVertices = mAllocator.Allocate<GLfloat>(mVertexCount);
    mIndices = mAllocator.Allocate<GLuint>(mIndexCount);

    memcpy(mVertices, vertices.begin(), vertices.size());
    memcpy(mIndices, indices.begin(), indices.size());
}

//----------------------------------------------------------------
void GraphicData::SetBuffers(const GLuint vertexBuffer, const GLuint elementBuffer) const
{
    // feed Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertexCount * sFloatByteSize, mVertices, GL_STATIC_DRAW);
    // feed Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexCount * sUintByteSize, mIndices, GL_STATIC_DRAW);
}

//********************************
//     Class Program
//********************************
//----------------------------------------------------------------
Program::Program(const char* vertexPath, const char* fragmentPath)
{
    FileString code(vertexPath);
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
    CheckErrors(id, sVertex);
    return id;
}

//----------------------------------------------------------------
void Program::LinkProgram(const GLuint vertexID, const GLuint fragmentID)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexID);
    glAttachShader(ID, fragmentID);
    glLinkProgram(ID);
    CheckErrors(ID, sProgram);
}

//----------------------------------------------------------------
void Program::CheckErrors(GLuint shader, const char* type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != sProgram)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LOG_STDERR(type << " Shader compilation error: " << infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG_STDERR(type << "Linking error: " << infoLog);
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sVertexStrideSize, sVertexPointPtr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sVertexStrideSize, sVertexColorPtr);
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
    SetMatrix4f("model", graphicData.GetTransformPtr());

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

//********************************
//     Class Object
//********************************
//----------------------------------------------------------------
Object::Object()
    : mVertices()
    , mIndices()
    , mVerticesByteSize(0)
    , mIndicesByteSize(0)
    , mTransform(1.0f)
{}

//----------------------------------------------------------------
Object::Object(const VertexVector vertices, const IndexVector indices)
    : mVertices(vertices)
    , mIndices(indices)
    , mVerticesByteSize(vertices.size() * sizeof(GLfloat))
    , mIndicesByteSize(indices.size() * sizeof(GLuint))
    , mTransform(1.0f)
{}

//----------------------------------------------------------------
Object::Object(const VertexAttribsVector vertices, const IndexVector indices)
    : mVertices()
    , mIndices(indices)
    , mIndicesByteSize(indices.size() * sizeof(GLuint))
    , mTransform(1.0f)
{
    mVertices.reserve(vertices.size() * eVertexAttribs::COUNT);

    for (const VertexAttribs& vertex : vertices)
    {
        vertex.AppendToVertexVector(mVertices);
    }
    mVerticesByteSize = mVertices.size() * sizeof(GLfloat);
}

//----------------------------------------------------------------
void Object::LoadObject(const VertexVector vertices, const IndexVector indices)
{
    mVertices         = vertices;
    mVerticesByteSize = vertices.size() * sizeof(GLfloat);
    mIndices          = indices;
    mIndicesByteSize  = indices.size() * sizeof(GLuint);
}

//----------------------------------------------------------------
void Object::AppendObject(const Object& other)
{
    mVertices.insert(mVertices.end(), other.mVertices.begin(), other.mVertices.end());
    mIndices.insert(mIndices.end(), other.mIndices.begin(), other.mIndices.end());
    mVerticesByteSize += other.mVerticesByteSize;
    mIndicesByteSize += other.mIndicesByteSize;
}

} // namespace GL 
} // namespace GameDisplay