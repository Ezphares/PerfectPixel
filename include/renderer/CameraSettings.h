#pragma once

#include "bedrock/vectors.h"

namespace perfectpixel { namespace renderer {

struct CameraSettings
{
    enum ScaleMode
    {
        SCALE_STRETCH,
        SCALE_BARS,
        SCALE_ENLARGE_X,
        SCALE_ENLARGE_Y
    };

    bedrock::Vector2 m_center;
    bedrock::Vector2 m_size;
    ScaleMode m_scaleMode;
};

}} // namespace perfectpixel::renderer
