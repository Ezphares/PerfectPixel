#pragma once

#include <graphics/texture.h>

namespace perfectpixel {
	namespace graphics {

		class FrameBuffer
		{
		public:
			FrameBuffer(types::Point2 size);
			~FrameBuffer();

		public:
			void resize(types::Point2 size);

			void bind();
			Texture &getTexture();

		private:
			Texture m_texture;
			GLuint m_id;
			GLuint m_depthBuffer;

			types::Point2 m_size;


		};

	}
}

