#pragma once
#include <core/include/vmemory.h>
#include <thirdparty.h>
#include <map>
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
    using TextureDataPtr = unsigned char*;
    using TextureInt = int32_t;
    
    static const size_t sVectorInBytes = sizeof(Vector);
    static const size_t sMatrixTransformInBytes = sizeof(MatrixTransform);
    static const size_t sVertexElementInBytes = sizeof(VertexElement);
    static const size_t sIndexElementBytes = sizeof(IndexElement);

    static const Vector VECTOR_UP = Vector(0.0f, 1.0f, 0.0f);
}

struct TextureInfo
{
    GLuint id;
    Types::TextureInt width;
    Types::TextureInt height;
    Types::TextureInt nrChannels;
    Types::TextureDataPtr data;

    TextureInfo()
        : data(NULL)
        , nrChannels(0)
        , height(0)
        , width(0)
        , id(0)
    {}

    TextureInfo(const char* path)
        : TextureInfo()
    {
        
        data = stbi_load(path, &width, &height, &nrChannels, 0);
        LOG_STDOUT(sizeof(data));
        if (data == NULL)
        {
            LOG_STDERR("Failed to load texture '" << path << "'.");
            id = -1;
        }
    }

    ~TextureInfo() { stbi_image_free(data); }

    const bool CheckInfo() const { return data != NULL; }
};

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
        TEX_X,
        TEX_Y,
        SIZE,

        POINT_FIRST = POINT_X,
        COLOR_FIRST = COLOR_R,
        TEX_FIRST = TEX_X
    };

    static const size_t sStrideSize = SIZE * Types::sVertexElementInBytes;
    static const size_t sPointPointer = POINT_FIRST * Types::sVertexElementInBytes;
    static const size_t sColorPointer = COLOR_FIRST * Types::sVertexElementInBytes;
    static const size_t sTexturePointer = TEX_FIRST * Types::sVertexElementInBytes;

    AttribArrayPtr(GLfloat* data)
        : mPointer(data)
    {}
    
    inline Types::VertexElement& X()     { return mPointer[POINT_X]; }
    inline Types::VertexElement& Y()     { return mPointer[POINT_Y]; }
    inline Types::VertexElement& Z()     { return mPointer[POINT_Z]; }

    inline Types::VertexElement& Red()   { return mPointer[COLOR_R]; }
    inline Types::VertexElement& Green() { return mPointer[COLOR_G]; }
    inline Types::VertexElement& Blue()  { return mPointer[COLOR_B]; }

    inline Types::VertexElement& TextureX() { return mPointer[TEX_X]; }
    inline Types::VertexElement& TextureY() { return mPointer[TEX_Y]; }

    static const size_t ElementCount() { return eAttrib::SIZE; }

};

namespace Types { using TextureMap = std::map<std::string, TextureInfo>; }    // Ordered set of texture names/info

class TextureLoader
{
private:
    
    TextureLoader();

    Types::TextureMap mTextureList;

    static TextureLoader* mInstance;

    static TextureInfo& iAddTexture(const char* path, const char* name = "unnamed");
    static const bool iRemoveTexture(const char* name);
    static const GLuint iGetTexture(const char* name);
    static Types::TextureMap& iGetTextureList();

public:
    static inline TextureInfo& AddTexture(const char* path, const char* name = "unnamed") 
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
    static inline Types::TextureMap& GetTextureList()
    {
        return iGetTextureList();
    }
};

class GraphicData
{
    GLenum mDrawMode = GL_TRIANGLES;

    std::vector<Types::VertexElement> mVertices;
    std::vector<Types::IndexElement> mIndices;
    std::vector<TextureInfo*> mTextures;
    Types::MatrixTransform mMatrixTransform;

public:
    GraphicData(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, std::initializer_list<const char*> texturePaths = {});

    void SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer);
    //void SetTextures()

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
    const bool LoadTextureToGL(TextureInfo& texture);
    void LoadAllTexturesToGL();
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