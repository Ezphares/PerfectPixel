#pragma once

#include <graphics/LocalGL.h>
#include <graphics/Texture.h>

#include <Bedrock/vectors.h>

namespace perfectpixel {
namespace graphics {

	class Sprite
	{


	public:
		Sprite(
			Texture *texture,
			bedrock::Vector2 topLeft,
			bedrock::Vector2 size,
			bedrock::PpInt frames = 1,
			bedrock::PpInt framesPerRow = 0,
			bedrock::Vector2 frameSpacing = bedrock::Vector2());
		~Sprite();

	public:
		bedrock::Vector2 getTexCoord(bedrock::PpInt frame = 0);
		bedrock::Vector2 getSize();
		Texture *getTexture();
		bedrock::PpInt getFrameNumber();

	private:
		Texture *m_texture;
		bedrock::Vector2 m_textureTopLeft;
		bedrock::Vector2 m_textureSize;

		bedrock::PpInt m_frames;
		bedrock::PpInt m_framesPerRow;
		bedrock::Vector2 m_frameSpacing;

	};

}
}