#include "core/UI/UILayoutTracker.h"

#include "bedrock/interpolation.h"

void perfectpixel::core::_internal::UILayoutTracker::makeRoot(
    bedrock::Vector2 absSize)
{
    m_layoutStack.resize(1);
    memset(reinterpret_cast<void *>(&m_layoutStack[0]), 0, sizeof(LayoutEntry));
    m_layoutStack[0].m_absMax = absSize;
}

void perfectpixel::core::_internal::UILayoutTracker::addLayout(
    bedrock::Vector2 localRelMin,
    bedrock::Vector2 localRelMax,
    LayoutDirection direction,
    float sign)
{
    LayoutEntry &top = m_layoutStack.back();

    bedrock::Vector2 toAdvance;
    if (top.m_cursorDirection.x > .5f)
    {
        toAdvance.x += localRelMax.x;
    }
    else if (top.m_cursorDirection.x < .5f)
    {
        toAdvance.x += localRelMin.x;
    }

    if (top.m_cursorDirection.y > .5f)
    {
        toAdvance.y += localRelMax.y;
    }
    else if (top.m_cursorDirection.y < .5f)
    {
        toAdvance.y += localRelMin.y;
    }

    LayoutEntry newEntry;

    newEntry.m_localRelMin  = localRelMin;
    newEntry.m_localRelMax  = localRelMax;
    newEntry.m_globalRelMin = {
        bedrock::invLerp(localRelMin.x, localRelMax.x, top.m_globalRelMin.x),
        bedrock::invLerp(localRelMin.y, localRelMax.y, top.m_globalRelMin.x)};
    newEntry.m_globalRelMax = {
        bedrock::invLerp(localRelMin.x, localRelMax.x, top.m_globalRelMin.x),
        bedrock::invLerp(localRelMin.y, localRelMax.y, top.m_globalRelMin.y)};
    newEntry.m_absMin = relToAbs(localRelMin);
    newEntry.m_absMax = relToAbs(localRelMax);
    newEntry.m_mouseLocal
        = {bedrock::invLerp(localRelMin.x, localRelMax.x, top.m_mouseLocal.x),
           bedrock::invLerp(localRelMin.y, localRelMax.y, top.m_mouseLocal.y)};
    newEntry.m_cursorDirection = bedrock::Vector2();
    if (direction == LayoutDirection::HORIZONTAL)
    {
        newEntry.m_cursorDirection = bedrock::Vector2::RIGHT;
    }
    else if (direction == LayoutDirection::VERTICAL)
    {
        newEntry.m_cursorDirection = bedrock::Vector2::UP;
    }
    newEntry.m_cursorDirection *= sign;
    newEntry.m_cursor
        = {newEntry.m_cursorDirection.x >= 0.0f ? 0.0f : 1.0f,
           newEntry.m_cursorDirection.y >= 0.0f ? 0.0f : 1.0f};

    m_layoutStack.push_back(std::move(newEntry));

    top.m_cursor = toAdvance;
}

perfectpixel::bedrock::Vector2
perfectpixel::core::_internal::UILayoutTracker::absToRel(
    const bedrock::Vector2 &absolute)
{
    const LayoutEntry &top = m_layoutStack.back();

    const bedrock::Vector2 scale{top.m_absMax - top.m_absMin};
    const bedrock::Vector2 local{absolute - top.m_absMin};

    return bedrock::Vector2{local.x / scale.x, local.y / scale.y};
}

perfectpixel::bedrock::Vector2
perfectpixel::core::_internal::UILayoutTracker::relToAbs(
    const bedrock::Vector2 &relative)
{
    const LayoutEntry &top = m_layoutStack.back();

    const bedrock::Vector2 scale{top.m_absMax - top.m_absMin};

    return top.m_absMin
           + bedrock::Vector2{relative.x * scale.x, relative.y * scale.y};
}

perfectpixel::bedrock::Vector2
perfectpixel::core::_internal::UILayoutTracker::cursor()
{
    return m_layoutStack.back().m_cursor;
}

void perfectpixel::core::_internal::UILayoutTracker::pop()
{
    if (m_layoutStack.size() > 1) // Do not pop the root
    {
        m_layoutStack.pop_back();
    }
}
