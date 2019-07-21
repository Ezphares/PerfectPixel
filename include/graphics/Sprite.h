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
			int32_t frames = 1,
			int32_t framesPerRow = 0,
			bedrock::Vector2 frameSpacing = bedrock::Vector2());
		~Sprite();

	public:
		bedrock::Vector2 getTexCoord(int32_t frame = 0);
		bedrock::Vector2 getSize();
		Texture *getTexture();
		int32_t getFrameNumber();

	private:
		Texture *m_texture;
		bedrock::Vector2 m_textureTopLeft;
		bedrock::Vector2 m_textureSize;

		int32_t m_frames;
		int32_t m_framesPerRow;
		bedrock::Vector2 m_frameSpacing;

	};

}
}