#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera(FrameBuffer& frameBuffer, const glm::vec3& pos, const glm::vec3& viewPoint, float fovy)
	: m_frameBuffer(frameBuffer), m_pos(pos)
{
	cameraFromClipMatrix = glm::inverse(glm::perspective(
		glm::radians(fovy),
		static_cast<float>(frameBuffer.getWidth()) / static_cast<float>(frameBuffer.getHeight()),
		1.0f, 
		2.0f
	));

	worldFromCameraMatrix = glm::inverse(
		glm::lookAt(
			pos,
			viewPoint, 
			glm::vec3(0.0f, 1.0f, 0.0f))
	);
}

Ray Camera::generateRay(const glm::ivec2 pixelCoord, const glm::vec2& offset) const
{
	glm::vec2 ndc = (glm::vec2(pixelCoord) + offset) / glm::vec2(m_frameBuffer.getWidth(), m_frameBuffer.getHeight());
	ndc.y = 1.0f - ndc.y;
	ndc = (ndc * 2.0f) - 1.0f;
	//( x, y, 0.0f, near) -> (x/near, y/near, 0.0f)  near = 1.0f
	glm::vec4 clip = glm::vec4(ndc, 0.0f, 1.0f);
	glm::vec3 world = worldFromCameraMatrix * cameraFromClipMatrix * clip;

	return Ray(
		m_pos,
		glm::normalize(world - m_pos)
	);
}
