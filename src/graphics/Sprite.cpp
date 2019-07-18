#include <graphics/Sprite.h>

namespace perfectpixel {
	namespace graphics {


		Sprite::Sprite(
			Texture *texture,
			bedrock::Vector2 topLeft,
			bedrock::Vector2 size,
			bedrock::PpInt frames /*= 1*/,
			bedrock::PpInt framesPerRow /*= 0*/,
			bedrock::Vector2 frameSpacing /*= types::Vector2()*/)
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

		perfectpixel::bedrock::Vector2 Sprite::getTexCoord(bedrock::PpInt frame /*= 0*/)
		{
			frame %= m_frames;
			bedrock::Vector2 step = m_textureSize + m_frameSpacing;

			return bedrock::Vector2{
				m_textureTopLeft.x() + (frame % m_framesPerRow) * step.x(),
				m_textureTopLeft.y() + (frame / m_framesPerRow) * step.y()
			};
		}

		perfectpixel::bedrock::Vector2 Sprite::getSize()
		{
			return m_textureSize;
		}

		Texture *Sprite::getTexture()
		{
			return m_texture;
		}

		perfectpixel::bedrock::PpInt Sprite::getFrameNumber()
		{
			return m_frames;
		}

	}
}
