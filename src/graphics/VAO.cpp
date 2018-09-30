#include <graphics/VAO.h>

#include <types/PpException.h>

namespace perfectpixel {
	namespace graphics {

		VAO::VAO(const BufferLayout &layout, bool interleavedBuffer /*= true*/)
		{
			m_numBuffers = interleavedBuffer ? 1 : layout.size();

			m_buffers.resize(m_numBuffers, 0);

			glGenVertexArrays(1, &m_id);
			glBindVertexArray(m_id);

			glGenBuffers(m_numBuffers, m_buffers.data());

			GLsizei stride = 0;
			if (interleavedBuffer)
			{
				for (auto &element : layout)
				{
					stride += getTotalSize(element);
				}
			}

			GLsizei offset = 0;
			for (std::size_t i = 0; i < layout.size(); ++i)
			{
				GLsizei bytes = getTotalSize(layout[i]);

				if (i == 0 || !interleavedBuffer)
				{
					glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]);
				}

				glEnableVertexAttribArray(i);
				glVertexAttribPointer(
					i, // layout index
					getNumElements(layout[i]), // Element number
					GL_FLOAT, // Element type
					GL_FALSE, // Normalize?
					stride, // Bytes between start of elements
					(GLvoid*)offset); // Offset of first element

				if (interleavedBuffer)
				{
					offset += bytes;
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		VAO::~VAO()
		{
			glDeleteBuffers(m_buffers.size(), m_buffers.data());
			glDeleteVertexArrays(1, &m_id);
		}

		void VAO::bindVAO()
		{
			glBindVertexArray(m_id);
		}

		void VAO::bindBuffer(GLuint index /*= 0*/)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[index % m_buffers.size()]);
		}

		GLsizei VAO::getTotalSize(BufferedElement element)
		{
			switch (element)
			{
			case BE_FLOAT:
				return sizeof(GLfloat);
			case BE_VEC_2:
				return sizeof(GLfloat) * 2;
			case BE_VEC_3:
				return sizeof(GLfloat) * 3;
			case BE_VEC_4:
				return sizeof(GLfloat) * 4;
			}

			throw types::PpException("Invalid element type");
		}

		GLsizei VAO::getNumElements(BufferedElement element)
		{
			switch (element)
			{
			case BE_FLOAT:
				return 1;
			case BE_VEC_2:
				return 2;
			case BE_VEC_3:
				return 3;
			case BE_VEC_4:
				return 4;
			}

			throw types::PpException("Invalid element type");
		}

	}
}
