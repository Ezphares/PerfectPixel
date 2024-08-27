#pragma once

#include <string>
#include <utility>

namespace perfectpixel { namespace renderer {

typedef std::pair<unsigned, unsigned> WindowDimensions;

struct WindowSettings
{
    enum WindowType
    {
        WINDOWED,
        BORDERLESS,
        FULLSCREEN
    };

    WindowDimensions dimensions;
    std::string title;
    WindowType type;
};

}} // namespace perfectpixel::renderer
