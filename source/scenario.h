#pragma once

#include <graphic.h>
#include <system/include/types.h>
#include <thirdparty.h>
#include <vector>


namespace Vision
{
namespace Scenario
{

class Camera
{
    using Vector = System::Types::Vector3;
    using Matrix = System::Types::Matrix44;
    
    Vector mPosition;
    Matrix mView;

public:
    Camera();

    void Move(const Vector& direction);
    void RotateAround(const Vector rotationVector)
    {
        mView = glm::translate(mView, rotationVector);
        mView = glm::rotate(mView, 0.10f, rotationVector);
        mView = glm::translate(mView, -rotationVector);

    }
    inline void ZoomIn()
    {
        mView = glm::scale(mView, Vector(1.1));
    }
    inline void ZoomOut()
    {
        mView = glm::scale(mView, Vector(0.9));
    }
    const Matrix& GetView() const;
};

class Object
{
    using GraphicData = Graphic::GraphicData;

    bool mHidden = false;
    GraphicData mGraphicData;

public:
    Object()
    {}

    Object(const GraphicData& graphicData)
        : mHidden(false)
        , mGraphicData(graphicData)
    {}

    inline const bool IsHidden() { return mHidden; }
    inline void SetHidden(const bool val) { mHidden = val; }
    inline const GraphicData& GetGraphicData() { return mGraphicData; }
    inline void SetGraphicData(const GraphicData val) { mGraphicData = val; }
};

class Scenario
{
    using DrawingInfo = System::DrawingInfo;
    using ObjectVector = std::vector<Object>;
    using CameraVector = std::vector<Camera>;
    using Matrix = System::Types::Matrix44;

    ObjectVector mObjects;
    CameraVector mCameras;
    int mCurrentCamera;
    DrawingInfo mDrawingInfo;

    void GenDrawingInfo();

public: 
    Scenario();
    ~Scenario();
    void LoadObject(Object& object);
    void HideObject(const int index, const bool hidden = true);
    const DrawingInfo& GetDrawingInfo();
    
    void SetAllBuffers(System::Types::UInt& vertexBuffer, System::Types::UInt& elementBuffer);
    
    inline Camera& GetCurrentCamera() { return mCameras.at(mCurrentCamera); }
    inline const Matrix& GetCurrentCameraView() { return mCameras.at(mCurrentCamera).GetView(); }
};

} // namespace Scenario
} // namespace Vision