#pragma once

#include "bedrock/Reflect.h"
#include "bedrock/vectors.h"

namespace perfectpixel { namespace renderer {

using ImageResourceBundleID = int32_t;

struct ImageResource
{
    int x, y, channels;
    char *buffer;
};

struct ImageResourceUserData
{
    ImageResourceBundleID bundleID;
};

}} // namespace perfectpixel::renderer

PP_REFLECT(::perfectpixel::renderer::ImageResource);
