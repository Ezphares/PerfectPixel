#pragma once

#include "bedrock/vectors.h"

#include <vector>

namespace perfectpixel { namespace renderer { namespace _internal {

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
        bedrock::Vector2 m_cursorDirection;
    };

    void makeRoot(bedrock::Vector2 absSize);

    void addLayout(
        bedrock::Vector2 localRelMin,
        bedrock::Vector2 localRelMax,
        LayoutDirection direction,
        float sign);

    bedrock::Vector2 absToRel(const bedrock::Vector2 &absolute);
    bedrock::Vector2 relToAbs(const bedrock::Vector2 &relative);
    bedrock::Vector2 cursor();

    void pop();

private:
    std::vector<LayoutEntry> m_layoutStack;
};

}}} // namespace perfectpixel::renderer::_internal
