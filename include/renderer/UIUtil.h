#pragma once

namespace perfectpixel { namespace renderer {

class UIUtil
{
public:
    enum TextAlignment
    {
        UITA_LEFT   = (1 << 0),
        UITA_CENTER = (1 << 1),
        UITA_RIGHT  = (1 << 2),

        UITA_TOP    = (1 << 3),
        UITA_MIDDLE = (1 << 4),
        UITA_BOTTOM = (1 << 5),
    };
};

}} // namespace perfectpixel::renderer
