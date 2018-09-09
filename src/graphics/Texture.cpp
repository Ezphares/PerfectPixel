#include <graphics/Texture.h>

#include <types/PpException.h>

#include <zlib.h>
#include <png.h>

namespace perfectpixel {
	namespace graphics {




Texture::Texture(const PNG &png)
{
	glGenTextures(1, &m_textureId);
	if (&m_textureId == 0)
	{
		throw types::PpException("Could not generate texture");
	}

	bind();

	GLint color_format = png.m_channels == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		color_format,
		static_cast<GLsizei>(png.m_w),
		static_cast<GLsizei>(png.m_h),
		0,
		color_format,
		GL_UNSIGNED_BYTE,
		static_cast<GLvoid*>(png.m_buffer.m_idata));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	m_size = types::Point2(
		static_cast<types::PpInt>(png.m_w),
		static_cast<types::PpInt>(png.m_h));
}

Texture::~Texture()
{
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

perfectpixel::types::Vector2 Texture::pixelToTexture(types::Point2 pixel) const
{
	return types::Vector2{
		static_cast<types::PpFloat>(pixel.m_x) / m_size.m_x,
		static_cast<types::PpFloat>(pixel.m_y) / m_size.m_y,
	};
}

perfectpixel::types::Point2 Texture::textureToPixel(types::Vector2 pixel) const
{
	return types::Point2{
		static_cast<types::PpInt>(pixel.m_x * m_size.m_x + 0.001),
		static_cast<types::PpInt>(pixel.m_y * m_size.m_y + 0.001),
	};
}

GLuint Texture::getId() const
{
	return m_textureId;
}

}
}
