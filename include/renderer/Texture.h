#pragma once

#include "renderer/LocalGL.h"

#include "renderer/ImageResource.h"

#include "bedrock/vectors.h"

#include <string>

namespace perfectpixel { namespace renderer {

// Forward declared
struct CBFGFontHeader;

class Texture
{
public:
    struct PlaceHolder
    {};
    static const PlaceHolder PLACEHOLDER;

public:
    Texture(const PlaceHolder &);
    Texture(const ImageResource &image, int32_t imageResourceId = 0);
    Texture(const bedrock::Point2 size);
    Texture(const CBFGFontHeader &header, const char *raw);

    ~Texture();

public:
    void bind(GLuint unit = 1);
    static void unbind(GLuint unit = 1);
    bedrock::Vector2 pixelToTexture(bedrock::Point2 pixel) const;
    bedrock::Point2 textureToPixel(bedrock::Vector2 pixel) const;

    GLuint getId() const;

    int32_t getSourceImageId() const;
    void destroy();

private:
    GLuint m_textureId;
    bedrock::Point2 m_size;
    int32_t m_sourceImageId;
};

}} // namespace perfectpixel::renderer
