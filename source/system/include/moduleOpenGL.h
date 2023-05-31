#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <gdmemory.h>

namespace GameDisplay
{
namespace GL 
{
    enum eVertexAttribs
    {
        POINT_X,
        POINT_Y,
        POINT_Z,
        COLOR_R,
        COLOR_G,
        COLOR_B,
        COUNT,

        POINT_FIRST = POINT_X,
        COLOR_FIRST = COLOR_R
    };

    typedef std::vector<GLfloat> VertexVector;
    typedef std::vector<GLuint> IndexVector;
    typedef glm::mat<4, 4, GLfloat> TransformMatrix;
    typedef glm::vec3 Vector3;

    static const char* const sFragment  = "Fragment";
    static const char* const sVertex    = "Vertex";
    static const char* const sProgram   = "Program";
    static const int sVertexStrideSize = eVertexAttribs::COUNT * sizeof(GLfloat);
    static const auto sVertexPointPtr = (void*)(eVertexAttribs::POINT_FIRST * sizeof(GLfloat));
    static const auto sVertexColorPtr = (void*)(eVertexAttribs::COLOR_FIRST * sizeof(GLfloat));
    static const int sFloatByteSize = sizeof(GLfloat);
    static const int sUintByteSize = sizeof(GLuint);

    struct VertexAttribs
    {
        // Point
        GLfloat x;
        GLfloat y;
        GLfloat z;
        // Color
        GLfloat r;
        GLfloat g;
        GLfloat b;
        // C'tor
        VertexAttribs(
            const GLfloat pointX = 0.0f, 
            const GLfloat pointY = 0.0f, 
            const GLfloat pointZ = 0.0f, 
            const GLfloat colorR = 0.0f,
            const GLfloat colorG = 0.0f,
            const GLfloat colorB = 0.0f)
            : x(pointX)
            , y(pointY)
            , z(pointZ)
            , r(colorR)
            , g(colorG)
            , b(colorB)
        {}

        VertexAttribs(std::initializer_list<GLfloat> list)
        {
            GLfloat* unsafe_ptr = &x;
            for (auto& number : list)
            {
                *unsafe_ptr = number;
                ++unsafe_ptr;
            }
        }

        inline const VertexVector& AppendToVertexVector(VertexVector& vertexVector) const
        {
            vertexVector.push_back(x);
            vertexVector.push_back(y);
            vertexVector.push_back(z);
            vertexVector.push_back(r);
            vertexVector.push_back(g);
            vertexVector.push_back(b);
            return vertexVector;
        }
        
        inline void CopyPosition(GLfloat* vArray)
        {
            x = vArray[eVertexAttribs::POINT_X];
            y = vArray[eVertexAttribs::POINT_Y];
            z = vArray[eVertexAttribs::POINT_Z];
        }
        inline void CopyColor(GLfloat* vArray)
        {
            r = vArray[eVertexAttribs::COLOR_R];
            g = vArray[eVertexAttribs::COLOR_G];
            b = vArray[eVertexAttribs::COLOR_B];
        }
        inline void Copy(GLfloat* vArray)
        {
            CopyPosition(vArray);
            CopyColor(vArray);
        }
    };

    typedef std::vector<VertexAttribs> VertexAttribsVector;

    /**
     *  \brief Graphic object, containing vertices and indices of an object to be drawn.
     * This object is meant to manage info for OpenGL to draw from triangles, indexed mode
     *  glDrawElements(GL_TRIANGLES, ...)
     */
    class Object
    {
        GLsizeiptr mVerticesByteSize;
        GLsizeiptr mIndicesByteSize;
        TransformMatrix mTransform;
    protected:
        VertexVector mVertices;
        IndexVector  mIndices;
    public:
        Object();
        Object(const VertexVector vertices, const IndexVector indices);
        Object(const VertexAttribsVector vertices, const IndexVector indices);

        inline const size_t     VertexCount()       { return mVertices.size(); }
        inline const size_t     IndexCount()        { return mIndices.size(); }
        inline const GLsizeiptr VerticesByteSize()  { return mVerticesByteSize; }
        inline const GLsizeiptr IndicesByteSize()   { return mIndicesByteSize; }
        inline const GLfloat*   GetVerticesData()   { return mVertices.data(); }
        inline const GLuint*    GetIndicesData()    { return mIndices.data(); }
        inline TransformMatrix& GetTransform()      { return mTransform; }
        inline GLfloat*         GetTransformPtr()   { return glm::value_ptr(mTransform); }

        inline       void       Translate(const Vector3 position)
        {
            mTransform = glm::translate(mTransform, position);
        }
        inline       void       SetTranslation(const Vector3 position)
        {
            mTransform = glm::translate(glm::mat4(1.0f), position);
        }

        inline       void       Rotate(const GLfloat angle, const Vector3 axis)
        {
            mTransform = glm::rotate(mTransform, angle, axis);
        }
        inline       void       SetRotation(const GLfloat angle, const Vector3 axis)
        {
            mTransform = glm::rotate(glm::mat4(1.0f), angle, axis);
        }

        virtual void LoadObject(const VertexVector vertices, const IndexVector indices);
        virtual void AppendObject(const Object& other);
    };

class AttribArrayPtr
{
    GLfloat* mPointer;
public:
    enum eAttrib
    {
        POINT_X,
        POINT_Y,
        POINT_Z,
        COLOR_R,
        COLOR_G,
        COLOR_B,
        SIZE
    };

    AttribArrayPtr(GLfloat* data)
        : mPointer(data)
    {}
    
    inline GLfloat& X() { return mPointer[POINT_X]; }
    inline GLfloat& Y() { return mPointer[POINT_Y]; }
    inline GLfloat& Z() { return mPointer[POINT_Z]; }

    inline GLfloat& Red()   { return mPointer[COLOR_R]; }
    inline GLfloat& Green() { return mPointer[COLOR_G]; }
    inline GLfloat& Blue()  { return mPointer[COLOR_B]; }

    static const size_t Size() { return eAttrib::SIZE; }
};

class GraphicData
{
    Memory::Allocator& mAllocator;
    GLfloat* mVertices;
    GLuint* mIndices;
    TransformMatrix mTransform;
    size_t mVertexCount;
    size_t mIndexCount;
    size_t mDrawMode;

public:
    /* Constructor: requires an allocator, maybe from the scene. */
    GraphicData(Memory::Allocator& allocator);

    /* Constructor: takes an allocator and a list of vertices and indices, optionally give a Transform matrix and a draw mode (default GL_TRIANGLES) */
    GraphicData(Memory::Allocator& allocator,
        std::initializer_list<GLfloat> vertices,
        std::initializer_list<GLuint> indices,
        const TransformMatrix transform = TransformMatrix(1.0f),
        const size_t drawMode = GL_TRIANGLES);

    /* Loads model from a list of vertices and indices, optionally give a Transform matrix and a draw mode (default GL_TRIANGLES) */
    void Load(std::initializer_list<GLfloat> vertices,
        std::initializer_list<GLuint> indices,
        const TransformMatrix transform,
        size_t drawMode = GL_TRIANGLES);

    /* Sets GL buffers with the corresponding vertex and index data. */
    void SetBuffers(const GLuint vertexBuffer, const GLuint elementBuffer) const;
    
    /* Return the number of indexes */
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
};

    class Program
    {
        GLuint mVertexArrayObject;
        GLuint mVertexBufferObject;
        GLuint mElementArrayBuffer;
         
        const GLuint CompileShader(const char* code, const GLuint type);
        void LinkProgram(const GLuint vertexID, const GLuint fragmentID);
        void CheckErrors(GLuint shader, const char* type);
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

} // namespace GL 
} // namespace GameDisplay