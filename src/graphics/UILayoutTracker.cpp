#include "graphics/UILayoutTracker.h"

void perfectpixel::graphics::_internal::UILayoutTracker::makeRoot(
    bedrock::Vector2 absSize)
{
    m_layoutStack.resize(1);
    memset(reinterpret_cast<void *>(&m_layoutStack[0]), 0, sizeof(LayoutEntry));
    m_layoutStack[0].m_absMax = absSize;
}

perfectpixel::bedrock::Vector2
perfectpixel::graphics::_internal::UILayoutTracker::absToRel(
    const bedrock::Vector2 &absolute)
{
    const LayoutEntry &top = m_layoutStack.back();

    const bedrock::Vector2 scale{top.m_absMax - top.m_absMin};
    const bedrock::Vector2 local{absolute - top.m_absMin};

    return bedrock::Vector2{local.x() / scale.x(), local.y() / scale.y()};
}

perfectpixel::bedrock::Vector2
perfectpixel::graphics::_internal::UILayoutTracker::relToAbs(
    const bedrock::Vector2 &relative)
{
    const LayoutEntry &top = m_layoutStack.back();

    const bedrock::Vector2 scale{top.m_absMax - top.m_absMin};

    return top.m_absMin
           + bedrock::Vector2{
               relative.x() * scale.x(), relative.y() * scale.y()};
}

void perfectpixel::graphics::_internal::UILayoutTracker::pop()
{
    if (m_layoutStack.size() > 1) // Do not pop the root
    {
        m_layoutStack.pop_back();
    }
}
