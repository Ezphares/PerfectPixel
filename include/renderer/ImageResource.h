#pragma once

#include "bedrock/TypeReflection.h"
#include "bedrock/vectors.h"

namespace perfectpixel { namespace renderer {

struct ImageResource
{
public:
    int x, y, channels;
    char *buffer;
};
}} // namespace perfectpixel::renderer

PP_TYPE_REFLECTION(::perfectpixel::renderer::ImageResource, ImageResource);
