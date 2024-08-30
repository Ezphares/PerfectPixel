#pragma once

#include "renderer/LocalGL.h"

#include <vector>

namespace perfectpixel { namespace renderer {

class VAO
{
public:
    enum class BufferedElement : uint8_t
    {
        Float,
        Vector2,
        Vector3,
        Vector4,
    };

    typedef std::vector<BufferedElement> BufferLayout;

public:
    VAO(const BufferLayout &layout, bool interleavedBuffer = true);
    ~VAO();

public:
    void bindVAO();
    static void unbindVAO();

    void bindBuffer(GLuint index = 0);
    static void unbindBuffer();

private:
    GLsizei getTotalSize(BufferedElement element);
    GLsizei getNumElements(BufferedElement element);

private:
    std::size_t m_numBuffers;
    GLuint m_id;
    std::vector<GLuint> m_buffers;
};

}} // namespace perfectpixel::renderer
