#include <thirdparty/include/thirdparty.h>

namespace Vision
{
namespace System
{
namespace Types
{

typedef glm::vec3 Vector3;
typedef glm::vec4 Vector4;
typedef glm::mat4 Matrix44;
typedef GLfloat   Float;
typedef GLuint    UInt;

typedef std::vector<Float> VertexVector;

static const Vector3 VECTOR_UP = Vector3(0.0f, 1.0f, 0.0f);
   
namespace VertexConst
{
    enum eKey
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

    static const size_t sTransformMatrixInBytes = sizeof(Matrix44);
    static const size_t sVertexElementInBytes   = sizeof(Float);
    static const size_t sIndexElementBytes      = sizeof(UInt);

    static const size_t sStrideSize             = SIZE * sVertexElementInBytes;
    static const size_t sPointPointer           = POINT_FIRST * sVertexElementInBytes;
    static const size_t sColorPointer           = COLOR_FIRST * sVertexElementInBytes;
    static const size_t sTexturePointer         = TEX_FIRST * sVertexElementInBytes;

} // namespace VertexConst

struct IVertexData
{
    virtual Float& GetElement(const VertexConst::eKey& key) = 0;

// Position -----------------------------
    inline Float& X() { return GetElement(VertexConst::eKey::POINT_X); }
    inline Float& Y() { return GetElement(VertexConst::eKey::POINT_Y); }
    inline Float& Z() { return GetElement(VertexConst::eKey::POINT_Z); }
    inline void ChangePosition(const Types::Float x, const Types::Float y, const Types::Float z)
    {
        X() = x;
        Y() = y;
        Z() = z;
    }
    inline void ChangePosition(const Vector3 xyz)
    {
        ChangePosition(xyz.x, xyz.y, xyz.z);
    }

// Color ----------------------------------
    inline Float& Red()   { return GetElement(VertexConst::eKey::COLOR_R); }
    inline Float& Green() { return GetElement(VertexConst::eKey::COLOR_G); }
    inline Float& Blue()  { return GetElement(VertexConst::eKey::COLOR_B); }
    inline void ChangeColor(const Types::Float red, const Types::Float green, const Types::Float blue)
    {
        Red() = red;
        Green() = green;
        Blue() = blue;
    }
    inline void ChangeColor(const Vector3 rgb)
    {
        ChangeColor(rgb.r, rgb.g, rgb.b);
    }

// Texture Position ------------------------
    inline Float& TextureX() { return GetElement(VertexConst::eKey::TEX_X); }
    inline Float& TextureY() { return GetElement(VertexConst::eKey::TEX_Y); }
};

struct VertexData : IVertexData
{
    VertexVector* rawPointer;
    const UInt index;

    VertexData(VertexVector& dataPointer, const UInt index)
        : rawPointer(&dataPointer)
        , index(index)
    {}
    
    inline Float& GetElement(const VertexConst::eKey& key)
    {
        return rawPointer->at(key + index);
    }
};

struct TextureInfo
{
    GLuint id;
    int width;
    int height;
    int nrChannels;
    unsigned char* data;

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

}//namespace Types
}//namespace System
}//namespace Vision