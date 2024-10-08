#include "renderer/FrameBuffer.h"

#include "bedrock/Assert.h"

namespace perfectpixel { namespace renderer {

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
{}

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
    glRenderbufferStorage(
        GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_size.m_x, m_size.m_y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        m_texture.getId(),
        0);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

    const GLenum buffers[1]{GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, buffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    PP_ASSERT(
        result == GL_FRAMEBUFFER_COMPLETE, "Failed to create framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind()
{
    PP_ASSERT(
        m_size.m_x > 0 && m_size.m_y > 0,
        "Tried to bind zero-size framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, m_size.m_x, m_size.m_y);
}

perfectpixel::renderer::Texture &FrameBuffer::getTexture()
{
    return m_texture;
}
}} // namespace perfectpixel::renderer
