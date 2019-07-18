#pragma once

#include <graphics/texture.h>

namespace perfectpixel {
	namespace graphics {

		class FrameBuffer
		{
		public:
			FrameBuffer(bedrock::Point2 size);
			~FrameBuffer();

		public:
			void resize(bedrock::Point2 size);

			void bind();
			Texture &getTexture();

		private:
			Texture m_texture;
			GLuint m_id;
			GLuint m_depthBuffer;

			bedrock::Point2 m_size;


		};

	}
}

