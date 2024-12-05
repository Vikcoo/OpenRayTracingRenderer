#include "FrameBuffer.h"
#include <iostream>
#include <fstream>


FrameBuffer::FrameBuffer(size_t width, size_t height)
	: m_width(width), m_height(height)
{
	 m_pixels.resize(m_width * m_height);
}

void FrameBuffer::save(const std::filesystem::path& filename)
{
	std::ofstream file(filename);
	file << "P3\n" << m_width << " " << m_height << "\n255\n";
	for (size_t y = 0; y < m_height; y++) {
		std::cerr << "\rScanlines remaining: " << y * 100 / m_height  << "%" << ' ' << std::flush;
		for (size_t x = 0; x < m_width; x++) {
			const glm::vec3& pixel = getPixel(x, y);
			glm::ivec3 color = glm::clamp (pixel * 255.0f, 0.0f, 255.0f);
			file << color.r << " " << color.g << " " << color.b << "\n";
		}
	}
}

//void FrameBuffer::save(const std::filesystem::path& filename)
//{
//	std::ofstream file(filename, std::ios::binary);
//	file << "P6\n" << m_width << " " << m_height << "\n255\n";
//	for (size_t y = 0; y < m_height; y++) {
//		for (size_t x = 0; x < m_width; x++) {
//			const glm::vec3& pixel = getPixel(x, y);
//			glm::ivec3 color = glm::clamp(pixel * 255.0f, 0.0f, 255.0f);
//			file << static_cast<uint8_t>(color.r) << ' ' << static_cast<uint8_t>(color.g) <<' ' << static_cast<uint8_t>(color.b) << '\n';
//		}
//	}
//}
//


