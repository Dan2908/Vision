#include <graphic/include/graphic.h>
#include "fileManager.h"
#include <sstream>

namespace Vision
{
namespace Graphic
{
//********************************
//     Class GraphicData
//********************************
//----------------------------------------------------------------
GraphicData::GraphicData()
    : mVertices()
    , mIndices()
    , mTextures()
    , mMatrixTransform(1.0f)
{}

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
GraphicData::~GraphicData()
{
    mVertices.clear();
    mIndices.clear();
    mTextures.clear();
}

//----------------------------------------------------------------
void GraphicData::AddVertex(std::initializer_list<System::Types::Float> vertex)
{
    for (auto& v : vertex)
    {
        mVertices.push_back(v);
    }
}

//----------------------------------------------------------------
void GraphicData::AddIndex(std::initializer_list<System::Types::UInt> index)
{
    for (auto& i : index)
    {
        mIndices.push_back(i);
    }
}

//----------------------------------------------------------------
void GraphicData::AddTexture(const char* texturePath)
{
    mTextures.push_back(&TextureLoader::AddTexture(texturePath));
}

//----------------------------------------------------------------
void GraphicData::SetBuffers(GLuint& vertexBuffer, GLuint& elementBuffer) const
{
    using namespace System;
    // feed Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * Types::VertexConst::sVertexElementInBytes, mVertices.data(), GL_STATIC_DRAW);
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

/**
* @brief Reads 3D data from .OBJ files.
*/
class GraphicDataFile : Util::File<GraphicData>
{
    GraphicData* Read()
    {
        GraphicData* output = new GraphicData();

        char line[256];

        auto ReadVertex = [&line, &output]()
        {
            char* next = std::find(line, &line[256], ' ') + 1;
            output->AddVertex({ std::stof(next) }); //X
            next = std::find(next, &line[256], ' ') + 1;
            output->AddVertex({ std::stof(next) }); //Y
            next = std::find(next, &line[256], ' ') + 1;
            output->AddVertex({ std::stof(next) }); //Z
            output->AddVertex(
                {/* R     G     B     Tx    Ty*/
                    1.0f, 1.0f, 1.0f, 0.0f, 0.0f
                }
            );
        };

        std::stringstream inStream;
        inStream << mFile.rdbuf();

        while((inStream.gcount() > 0))
        {
            inStream.getline(line, 256);
            switch (line[0])
            {
            case 'v':
                ReadVertex();
                break;
            case 'vt':
               
                break;
            }
        }

        return output;
    }
};


//********************************
//     Class TextureLoader
//********************************

Graphic::TextureLoader* TextureLoader::mInstance = new TextureLoader();
//----------------------------------------------------------------
TextureLoader::TextureLoader()
    : mTextureList()
{
}

//----------------------------------------------------------------
System::Types::TextureInfo& TextureLoader::iAddTexture(const char* path, const char* name /*= "unnamed"*/)
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
    texList[rename] = *new System::Types::TextureInfo(path);

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

} // namespace Graphic 
} // namespace Vision