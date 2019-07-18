#include <graphics/FrameBuffer.h>

#include <Bedrock/PpException.h>

namespace perfectpixel {
	namespace graphics {

		FrameBuffer::FrameBuffer(bedrock::Point2 size)
			: m_texture(Texture::PLACEHOLDER)
			, m_id(0)
			, m_size()
		{
			glGenFramebuffers(1, &m_id);
			glGenRenderbuffers(1, &m_depthBuffer);

			resize(size);
		}

		FrameBuffer::~FrameBuffer()
		{
		}

		void FrameBuffer::resize(bedrock::Point2 size)
		{
			if (m_size == size)
			{
				return;
			}
			m_size = size;
			m_texture.destroy();

			if (m_size.m_x == 0 || m_size.m_y == 0)
			{
				m_texture = Texture(Texture::PLACEHOLDER);
				return;
			}

			// Regenerate texture
			m_texture = Texture(m_size);

			// Resize the depth buffer
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_size.m_x, m_size.m_y);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.getId(), 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

			const GLenum buffers[1]{ GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, buffers);

			GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (result != GL_FRAMEBUFFER_COMPLETE)
			{
				throw bedrock::PpException("Could not resize frame buffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FrameBuffer::bind()
		{
			if (m_size.m_x == 0 || m_size.m_y == 0)
			{
				throw bedrock::PpException("Cannot bind buffer with 0 component in size");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
			glViewport(0, 0, m_size.m_x, m_size.m_y);
		}

		perfectpixel::graphics::Texture & FrameBuffer::getTexture()
		{
			return m_texture;
		}
	}
}
