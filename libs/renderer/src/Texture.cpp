#include "renderer/Texture.h"

#include "renderer/CBFGFont.h"

#include "bedrock/PpException.h"

namespace perfectpixel { namespace renderer {

const Texture::PlaceHolder Texture::PLACEHOLDER = {};

Texture::Texture(const ImageResource &image, bedrock::ID imageResourceId)
    : m_sourceImageId(imageResourceId)
{
    glGenTextures(1, &m_textureId);
    if (m_textureId == 0)
    {
        throw bedrock::PpException("Could not generate texture");
    }

    bind();

    GLint color_format = image.channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        color_format,
        static_cast<GLsizei>(image.x),
        static_cast<GLsizei>(image.y),
        0,
        color_format,
        GL_UNSIGNED_BYTE,
        static_cast<GLvoid *>(image.buffer));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    m_size = bedrock::Point2(image.x, image.y);
}

Texture::Texture(const bedrock::Point2 size)
    : m_size(size)
    , m_sourceImageId(0)
{
    glGenTextures(1, &m_textureId);
    if (m_textureId == 0)
    {
        throw bedrock::PpException("Could not generate texture");
    }

    bind();
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        size.m_x,
        size.m_y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const PlaceHolder &)
    : m_textureId(0)
    , m_size()
    , m_sourceImageId({0})
{}

Texture::Texture(const CBFGFontHeader &header, const char *raw)
    : m_size(header.m_imageWidth, header.m_imageHeight)
{
    glGenTextures(1, &m_textureId);
    if (m_textureId == 0)
    {
        throw bedrock::PpException("Could not generate texture");
    }

    bind();

    GLint color_format
        = header.m_bpp == 8 ? GL_RED : (header.m_bpp == 24 ? GL_RGB : GL_RGBA);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        color_format,
        m_size.m_x,
        m_size.m_y,
        0,
        color_format,
        GL_UNSIGNED_BYTE,
        static_cast<const GLvoid *>(raw));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    unbind();
}

Texture::Texture(Texture &&rhs)
{
    m_textureId     = rhs.m_textureId;
    m_sourceImageId = rhs.m_sourceImageId;
    m_size          = rhs.m_size;

    rhs.m_textureId     = 0;
    rhs.m_size          = bedrock::Point2(0, 0);
    rhs.m_sourceImageId = bedrock::ID(0);
}

Texture &Texture::operator=(Texture &&rhs)
{
    if (&rhs != this)
    {
        m_textureId     = rhs.m_textureId;
        m_sourceImageId = rhs.m_sourceImageId;
        m_size          = rhs.m_size;

        rhs.m_textureId     = 0;
        rhs.m_size          = bedrock::Point2(0, 0);
        rhs.m_sourceImageId = bedrock::ID(0);
    }
    return *this;
}

Texture::~Texture()
{
    destroy();
}

void Texture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::unbind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

perfectpixel::bedrock::Vector2
Texture::pixelToTexture(bedrock::Point2 pixel) const
{
    return bedrock::Vector2{
        static_cast<float>(pixel.m_x) / m_size.m_x,
        static_cast<float>(pixel.m_y) / m_size.m_y,
    };
}

perfectpixel::bedrock::Point2
Texture::textureToPixel(bedrock::Vector2 pixel) const
{
    return bedrock::Point2{
        static_cast<int32_t>(pixel.x * m_size.m_x + 0.001),
        static_cast<int32_t>(pixel.y * m_size.m_y + 0.001),
    };
}

GLuint Texture::getId() const
{
    return m_textureId;
}

bedrock::ID Texture::getSourceImageId() const
{
    return m_sourceImageId;
}

void Texture::destroy()
{
    if (m_textureId != 0)
    {
        glDeleteTextures(1, &m_textureId);
    }
}

}} // namespace perfectpixel::renderer
