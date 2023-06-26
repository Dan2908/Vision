#pragma once

#include <system/include/moduleOpenGL.h>
#include <system/include/types.h>
#include <map>

namespace Vision
{
namespace Graphic
{

using TextureInfo = System::Types::TextureInfo;
using TextureMap = std::map<std::string, TextureInfo>;  // Ordered set of texture names/info

class TextureLoader
{
private:

    TextureLoader();

    TextureMap mTextureList;

    static TextureLoader* mInstance;

    static TextureInfo& iAddTexture(const char* path, const char* name = "unnamed");
    static const bool iRemoveTexture(const char* name);
    static const GLuint iGetTexture(const char* name);
    static TextureMap& iGetTextureList();

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
    static inline TextureMap& GetTextureList()
    {
        return iGetTextureList();
    }
};

class GraphicData
{
    using VertexVector = std::vector<System::Types::Float>;
    using IndexVector = std::vector<System::Types::UInt>;
    using TextureVector = std::vector<System::Types::TextureInfo*>;

    GLenum mDrawMode = GL_TRIANGLES;

    VertexVector mVertices;
    IndexVector mIndices;
    TextureVector mTextures;
    System::Types::Matrix44 mMatrixTransform;

public:
    GraphicData();
    GraphicData(std::initializer_list<GLfloat> vertices, std::initializer_list<GLuint> indices, std::initializer_list<const char*> texturePaths = {});
    ~GraphicData();

    void AddVertex(std::initializer_list<System::Types::Float> vertex);
    void AddIndex(std::initializer_list<System::Types::UInt> index);
    void AddTexture(const char* texturePaths);

    void SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer) const;

    inline VertexVector& GetVertexArray() { return mVertices; }
    inline const size_t GetIndexCount() const { return mIndices.size(); }
};

} // namespace Graphic 
} // namespace Vision