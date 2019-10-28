#pragma once

#include "Bedrock/vectors.h"

#include <vector>

namespace perfectpixel { namespace graphics { namespace _internal {

class UILayoutTracker
{
public:
    enum class LayoutDirection
    {
        NONE,
        HORIZONTAL,
        VERTICAL,
    };

    struct LayoutEntry
    {
        bedrock::Vector2 m_localRelMin;
        bedrock::Vector2 m_localRelMax;
        bedrock::Vector2 m_globalRelMin;
        bedrock::Vector2 m_globalRelMax;
        bedrock::Vector2 m_absMin;
        bedrock::Vector2 m_absMax;
        bedrock::Vector2 m_mouseLocal;
        bedrock::Vector2 m_cursor;
        LayoutDirection m_direction;
    };

    void makeRoot(bedrock::Vector2 absSize);

    bedrock::Vector2 absToRel(const bedrock::Vector2 &absolute);
    bedrock::Vector2 relToAbs(const bedrock::Vector2 &relative);

    void pop();

private:
    std::vector<LayoutEntry> m_layoutStack;
};

}}} // namespace perfectpixel::graphics::_internal
