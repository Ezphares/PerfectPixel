#include <graphics/Sprite.h>

namespace perfectpixel {
	namespace graphics {


		Sprite::Sprite(
			Texture *texture,
			types::Vector2 topLeft,
			types::Vector2 size,
			types::PpInt frames /*= 1*/,
			types::PpInt framesPerRow /*= 0*/,
			types::Vector2 frameSpacing /*= types::Vector2()*/)
			: m_texture(texture)
			, m_textureTopLeft(topLeft)
			, m_textureSize(size)
			, m_frames(frames)
			, m_framesPerRow(framesPerRow)
			, m_frameSpacing(frameSpacing)
		{
			if (m_framesPerRow <= 0) m_framesPerRow = m_frames;
		}

		Sprite::~Sprite()
		{
		}

		perfectpixel::types::Vector2 Sprite::getTexCoord(types::PpInt frame /*= 0*/)
		{
			frame %= m_frames;
			types::Vector2 step = m_textureSize + m_frameSpacing;

			return types::Vector2{
				m_textureTopLeft.x() + (frame % m_framesPerRow) * step.x(),
				m_textureTopLeft.y() + (frame / m_framesPerRow) * step.y()
			};
		}

		perfectpixel::types::Vector2 Sprite::getSize()
		{
			return m_textureSize;
		}

		Texture *Sprite::getTexture()
		{
			return m_texture;
		}

		perfectpixel::types::PpInt Sprite::getFrameNumber()
		{
			return m_frames;
		}

	}
}
