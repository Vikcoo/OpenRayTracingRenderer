#pragma once
#include "Ray.h"
#include "FrameBuffer.h"

class Camera
{
public:
    Camera(FrameBuffer& frameBuffer,const glm::vec3& pos, const glm::vec3& viewPoint, float fovy );
    Ray generateRay(const glm::ivec2 pixelCoord, const glm::vec2& offset = glm::vec2(0.5f, 0.5f)) const;

    const FrameBuffer& getFrameBuffer() const { return m_frameBuffer; }
    FrameBuffer& getFrameBuffer() { return m_frameBuffer; }
private:
    FrameBuffer& m_frameBuffer;
    glm::vec3 m_pos;

    glm::mat4 cameraFromClipMatrix;
    glm::mat4 worldFromCameraMatrix;
};