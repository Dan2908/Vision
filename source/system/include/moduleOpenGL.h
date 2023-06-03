#pragma once
#include <core/include/vmemory.h>
#include <thirdparty.h>
#include <vector>

namespace Vision
{
namespace System
{

namespace Types
{
    using Vector = glm::fvec3;
    using MatrixTransform = glm::mat4;
    using VertexElement = GLfloat;
    using IndexElement = GLuint;
    
    static const size_t sVectorInBytes = sizeof(Vector);
    static const size_t sMatrixTransformInBytes = sizeof(MatrixTransform);
    static const size_t sVertexElementInBytes = sizeof(VertexElement);
    static const size_t sIndexElementBytes = sizeof(IndexElement);
}

class AttribArrayPtr
{
    Types::VertexElement* mPointer;
public:
    enum eAttrib
    {
        POINT_X,
        POINT_Y,
        POINT_Z,
        COLOR_R,
        COLOR_G,
        COLOR_B,
        SIZE,

        POINT_FIRST = POINT_X,
        COLOR_FIRST = COLOR_R
    };

    AttribArrayPtr(GLfloat* data)
        : mPointer(data)
    {}
    
    inline Types::VertexElement& X()     { return mPointer[POINT_X]; }
    inline Types::VertexElement& Y()     { return mPointer[POINT_Y]; }
    inline Types::VertexElement& Z()     { return mPointer[POINT_Z]; }

    inline Types::VertexElement& Red()   { return mPointer[COLOR_R]; }
    inline Types::VertexElement& Green() { return mPointer[COLOR_G]; }
    inline Types::VertexElement& Blue()  { return mPointer[COLOR_B]; }

    static const size_t ElementCount() { return eAttrib::SIZE; }

    static const size_t sStrideSize = SIZE * Types::sVertexElementInBytes;
    static const size_t sPointPointer = POINT_FIRST * Types::sVertexElementInBytes;
    static const size_t sColorPointer = COLOR_FIRST * Types::sIndexElementBytes;
};

class GraphicData
{
    struct GraphicInfo
    {
        size_t vertexCount;
        size_t indexCount;
        size_t drawMode;
    };

    Core::Allocator& mAllocator;

    Types::VertexElement* mVertices;
    Types::IndexElement* mIndices;
    Types::MatrixTransform* mMatrixTransform;
    GraphicInfo* mGraphicInfo;

    void GenerateGraphicInfo(const size_t vertexCount, const size_t indexCount, const size_t drawMode);

public:
    /* Constructor (empty): requires an allocator, maybe from the scene. */
    GraphicData(Core::Allocator& allocator);

    /* Constructor: takes an allocator and a list of vertices and indices, optionally give a Transform matrix and a draw mode (default GL_TRIANGLES) */
    GraphicData(Core::Allocator& allocator,
        std::initializer_list<GLfloat> vertices,
        std::initializer_list<GLuint> indices,
        const Types::MatrixTransform transform = Types::MatrixTransform(1.0f),
        const size_t drawMode = GL_TRIANGLES);

    /* Sets GL buffers with the corresponding vertex and index data. */
    void SetBuffers(const GLuint vertexBuffer, const GLuint elementBuffer) const;

    inline Types::MatrixTransform& GetMatrixTransform() { return *mMatrixTransform; }
    inline size_t& GetVertexCount() { return mGraphicInfo->vertexCount; }
    inline size_t& GetIndexCount() { return mGraphicInfo->indexCount; }
    inline size_t& GetDrawMode() { return mGraphicInfo->drawMode; }

    inline Types::VertexElement*   GetMatrixTransformVPtr() { return glm::value_ptr(*mMatrixTransform); }
  /*
    // Return the number of indexes 
    inline const size_t GetIndexCount() { return mIndexCount; }

    inline GLfloat* GetTransformPtr() { return glm::value_ptr(mTransform); }
    inline void Rotate(const GLfloat angle, const Vector3 axis)
    {
        mTransform = glm::rotate(mTransform, angle, axis);
    }
    inline void SetRotation(const GLfloat angle, const Vector3 axis)
    {
        mTransform = glm::rotate(glm::mat4(1.0f), angle, axis);
    }
  */  
};

class Program
{
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mElementArrayBuffer;
     
    const GLuint CompileShader(const char* code, const GLuint type);
    void LinkProgram(const GLuint vertexID, const GLuint fragmentID);
    void CheckErrors(GLuint shader, const bool isShader);
    void CleanShaders(const GLuint vertexID, const GLuint fragmentID);
    void GenerateBuffers();

public:
    GLuint ID;

    Program(const char* vertexPath, const char* fragmentPath);
    void Use() const;
    void Draw(GraphicData& graphicData);
    void SetMatrix4f(const char* name, GLfloat* value_ptr);
};

class Camera
{
public:
    Camera();
};

} // namespace System 
} // namespace Vision