#include <scenario.h>

namespace Vision
{
namespace Scenario
{

using Vector = System::Types::Vector3;
using Matrix = System::Types::Matrix44;

//********************************
//     Class Camera
//********************************
//----------------------------------------------------------------
Camera::Camera()
    : mPosition(0.0f, 0.0f, 10.0f)
    , mView()
{
    mView = glm::lookAt(mPosition, Vector(0.0f), System::Types::VECTOR_UP);
}

//----------------------------------------------------------------
void Camera::Move(const Vector& direction)
{
    mPosition += direction;
    mView = glm::lookAt(mPosition, Vector(0.0f), System::Types::VECTOR_UP);
}

//----------------------------------------------------------------
const Matrix& Camera::GetView() const
{
    return mView;
}

//********************************
//     Class Scenario
//********************************
//----------------------------------------------------------------
Scenario::Scenario()
    : mObjects()
    , mCameras({ Camera() })
    , mCurrentCamera(0)
    , mDrawingInfo()
{}

//----------------------------------------------------------------
Scenario::~Scenario()
{
    mObjects.clear();
    mCameras.clear();
}

//----------------------------------------------------------------
void Scenario::LoadObject(Object& object)
{
    mObjects.push_back(object);
    mDrawingInfo.indexCount += object.GetGraphicData().GetIndexCount();
}

//----------------------------------------------------------------
void Scenario::HideObject(const int index, const bool hidden /*= true */)
{
    assert(index < mObjects.size());
    mObjects.at(index).SetHidden(hidden);
}

//----------------------------------------------------------------
const System::DrawingInfo& Scenario::GetDrawingInfo()
{
    return mDrawingInfo;
}

//----------------------------------------------------------------
void Scenario::GenDrawingInfo()
{
    // TODO: adapt this to take graphicData's drawType
    mDrawingInfo.drawType = System::eDrawType::TRIANGLES;

    mDrawingInfo.indexCount = 0;
    for (auto& object : mObjects)
    {
        if (!object.IsHidden())
        {
            mDrawingInfo.indexCount += object.GetGraphicData().GetIndexCount();
        }
    }
}

//----------------------------------------------------------------
void Scenario::SetAllBuffers(System::Types::UInt& vertexBuffer, System::Types::UInt& elementBuffer)
{
    for (auto& object : mObjects)
    {
        object.GetGraphicData().SetBuffers(vertexBuffer, elementBuffer);
    }
}

} // namespace Scenario
} // namespace Vision