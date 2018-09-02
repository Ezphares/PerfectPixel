#pragma once

#include <graphics/LocalGL.h>
#include <graphics/Texture.h>

#include <types/vectors.h>

namespace perfectpixel {
namespace graphics {

	class Sprite
	{


	public:
		Sprite(
			Texture *texture,
			types::Vector2 topLeft,
			types::Vector2 size,
			types::PpInt frames = 1,
			types::PpInt framesPerRow = 0,
			types::Vector2 frameSpacing = types::Vector2());
		~Sprite();

	public:
		types::Vector2 getTexCoord(types::PpInt frame = 0);
		types::Vector2 getSize();
		Texture *getTexture();
		types::PpInt getFrameNumber();

	private:
		Texture *m_texture;
		types::Vector2 m_textureTopLeft;
		types::Vector2 m_textureSize;

		types::PpInt m_frames;
		types::PpInt m_framesPerRow;
		types::Vector2 m_frameSpacing;

	};

}
}