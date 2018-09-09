#pragma once

#include <graphics/LocalGL.h>

#include <vector>

namespace perfectpixel {
	namespace graphics {

		class VAO
		{
		public:
			enum BufferedElement {
				BE_FLOAT,
				BE_VEC_2,
				BE_VEC_3,
				BE_VEC_4
			};

			typedef std:: vector<BufferedElement> BufferLayout;

		public:
			VAO(const BufferLayout &layout, bool interleavedBuffer = true);
			~VAO();

		public:
			void bindVAO();
			void bindBuffer(GLuint index = 0);

		private:
			GLsizei getTotalSize(BufferedElement element);
			GLsizei getNumElements(BufferedElement element);

		private:
			std::size_t m_numBuffers;
			GLuint m_id;
			std::vector<GLuint> m_buffers;
		};

	}
}
