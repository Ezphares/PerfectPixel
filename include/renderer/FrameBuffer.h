#pragma once

#include "renderer/Texture.h"

namespace perfectpixel { namespace renderer {

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

}} // namespace perfectpixel::renderer
