#include "renderer/VAO.h"

#include "bedrock/Assert.h"

namespace perfectpixel { namespace renderer {

VAO::VAO(const BufferLayout &layout, bool interleavedBuffer /*= true*/)
{
    m_numBuffers = interleavedBuffer ? 1 : layout.size();

    m_buffers.resize(m_numBuffers, 0);

    glGenVertexArrays(1, &m_id);
    bindVAO();

    glGenBuffers(static_cast<GLsizei>(m_numBuffers), m_buffers.data());

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
            bindBuffer(static_cast<GLuint>(i));
        }

        glEnableVertexAttribArray(static_cast<GLuint>(i));
        glVertexAttribPointer(
            static_cast<GLuint>(i),    // layout index
            getNumElements(layout[i]), // Element number
            GL_FLOAT,                  // Element type
            GL_FALSE,                  // Normalize?
            stride,                    // Bytes between start of elements
            (GLvoid *)((intptr_t)offset));
        if (interleavedBuffer)
        {
            offset += bytes;
        }
    }

    unbindBuffer();
    unbindVAO();
}

VAO::~VAO()
{
    glDeleteBuffers(static_cast<GLsizei>(m_buffers.size()), m_buffers.data());
    glDeleteVertexArrays(1, &m_id);
}

void VAO::bindVAO()
{
    glBindVertexArray(m_id);
}

void VAO::unbindVAO()
{
    glBindVertexArray(0);
}

void VAO::bindBuffer(GLuint index /*= 0*/)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_buffers[index % m_buffers.size()]);
}

void VAO::unbindBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLsizei VAO::getTotalSize(BufferedElement element)
{
    switch (element)
    {
    case BufferedElement::Float:
        return sizeof(GLfloat);
    case BufferedElement::Vector2:
        return sizeof(GLfloat) * 2;
    case BufferedElement::Vector3:
        return sizeof(GLfloat) * 3;
    case BufferedElement::Vector4:
        return sizeof(GLfloat) * 4;
    default:
        PP_ASSERT(false, "Unsupported VAO element");
        return 0;
    }
}

GLsizei VAO::getNumElements(BufferedElement element)
{
    switch (element)
    {
    case BufferedElement::Float:
        return 1;
    case BufferedElement::Vector2:
        return 2;
    case BufferedElement::Vector3:
        return 3;
    case BufferedElement::Vector4:
        return 4;
    default:
        PP_ASSERT(false, "Unsupported VAO element");
        return 0;
    }
}

}} // namespace perfectpixel::renderer
