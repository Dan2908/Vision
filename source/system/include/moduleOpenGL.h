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

    static const Vector VECTOR_UP = Vector(0.0f, 1.0f, 0.0f);
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
    std::vector<Types::VertexElement> mVertices;
    std::vector<Types::IndexElement> mIndices;
    Types::MatrixTransform mMatrixTransform;
    GLenum mDrawMode = GL_TRIANGLES;

public:
    GraphicData(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices);

    void SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer);

    inline const size_t GetIndexCount() { return mIndices.size(); }
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

    Program(const char* vertexPath, const char* fragmentPath);
    void Use() const;
    void SetMatrix4f(const char* name, const Types::MatrixTransform& matrix);

    void Draw(GraphicData& graphicData);
};

class Camera
{
    Types::Vector mPosition;
    Types::MatrixTransform mView;

public:
    Camera();

    void Move(const Types::Vector& direction);

    const Types::MatrixTransform& GetView() const;
};

} // namespace System 
} // namespace Vision