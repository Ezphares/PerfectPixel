#pragma once

#include "renderer/BufferLayouts.h"

#include "bedrock/vectors.h"

#include <string>
#include <vector>

namespace perfectpixel { namespace renderer {

class Texture;

class IFont
{
public:
    virtual ~IFont(){};

    virtual void writeBuffer(
        bedrock::Vector3 position,
        float size,
        const std::string &text,
        std::vector<TextVertex> *out_vertices)
        = 0;
    virtual Texture &getTexture()                               = 0;
    virtual float getWidth(float size, const std::string &text) = 0;
};
}} // namespace perfectpixel::renderer
