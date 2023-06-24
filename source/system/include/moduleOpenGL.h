#pragma once
#include <core/include/vmemory.h>
#include <thirdparty.h>
#include <map>
#include <vector>
#include <system/include/types.h>

namespace Vision
{
namespace System
{

using TextureMap = std::map<std::string, Types::TextureInfo>;  // Ordered set of texture names/info

class TextureLoader
{
private:
    
    TextureLoader();

    TextureMap mTextureList;

    static TextureLoader* mInstance;

    static Types::TextureInfo& iAddTexture(const char* path, const char* name = "unnamed");
    static const bool iRemoveTexture(const char* name);
    static const GLuint iGetTexture(const char* name);
    static TextureMap& iGetTextureList();

public:
    static inline Types::TextureInfo& AddTexture(const char* path, const char* name = "unnamed") 
    {
        return iAddTexture(path, name);
    }
    static inline const bool RemoveTexture(const char* name)
    {
        return iRemoveTexture(name);
    }
    static inline const GLuint GetTexture(const char* name)
    {
        return iGetTexture(name);
    }
    static inline TextureMap& GetTextureList()
    {
        return iGetTextureList();
    }
};

class GraphicData
{
    using VertexVector  = std::vector<Types::Float>;
    using IndexVector   = std::vector<Types::UInt>;
    using TextureVector = std::vector<Types::TextureInfo*>;

    GLenum mDrawMode = GL_TRIANGLES;

    VertexVector mVertices;
    IndexVector mIndices;
    TextureVector mTextures;
    Types::Matrix44 mMatrixTransform;

public:
    GraphicData(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, std::initializer_list<const char*> texturePaths = {});



    void SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer);
    //void SetTextures()

    VertexVector& GetVertexArray()
    {
        return mVertices;
    }

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
    void SetMatrix4f(const char* name, const Types::Matrix44& matrix);
    const bool LoadTextureToGL(Types::TextureInfo& texture);
    void LoadAllTexturesToGL();
    void Draw(GraphicData& graphicData);
};



class Camera
{
    Types::Vector3 mPosition;
    Types::Matrix44 mView;

public:
    Camera();

    void Move(const Types::Vector3& direction);
    void RotateAround(const Types::Vector3 rotationVector)
    {
        mView = glm::translate(mView, rotationVector);
        mView = glm::rotate(mView, 0.10f, rotationVector);
        mView = glm::translate(mView, -rotationVector);

    }
    inline void ZoomIn() 
    { 
        mView = glm::scale(mView, Types::Vector3(1.1));
    }
    inline void ZoomOut()
    {
        mView = glm::scale(mView, Types::Vector3(0.9));
    }
    const Types::Matrix44& GetView() const;
};

} // namespace System 
} // namespace Vision