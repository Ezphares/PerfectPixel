
#include "renderer/CameraSettings.h"

#include "core/UI/GUI.h"
#include "core/UI/UILayoutTracker.h"
#include "core/UI/UIShadow.h"

namespace perfectpixel::core {
namespace _internal {

class GUIInternal
{
public:
    UILayoutTracker m_tracker;
    UIShadow m_shadow;
};

} // namespace _internal
GUI::AnchoredRegion GUI::AnchoredRegion::absolute(
    bedrock::Vector2 absSize,
    PositionHelper
        position /*= static_cast<PositionHelper>(ALIGN_LEFT | ALIGN_TOP)*/)
{
    AnchoredRegion result;

    result.m_extend = absSize;

    float v = 0.5f;
    float h = 0.5f;

    if ((position & ALIGN_LEFT) > 0)
    {
        h = 0.0f;
    }
    else if ((position & ALIGN_RIGHT) > 0)
    {
        h = 1.0f;
    }

    if ((position & ALIGN_BOTTOM) > 0)
    {
        v = 0.0f;
    }
    else if ((position & ALIGN_TOP) > 0)
    {
        v = 1.0f;
    }

    result.m_anchor.m_left = result.m_anchor.m_right = result.m_pivot.x = h;

    result.m_anchor.m_top = result.m_anchor.m_bottom = result.m_pivot.y = v;

    return result;
}

} // namespace perfectpixel::core

perfectpixel::core::GUI::HorizontalLayout::HorizontalLayout(
    const AnchoredRegion &position,
    LinearLayoutOptions options /*= LinearLayoutOptions()*/)
{
    GUI::pushHorizontalLayout(position, options);
}

perfectpixel::core::GUI::HorizontalLayout::~HorizontalLayout()
{
    GUI::popLayout();
}

void perfectpixel::core::GUI::Spacer(float size, bool relative /*= false*/)
{
    (void)size; // TODO
    (void)relative;
}

void perfectpixel::core::GUI::begin(const renderer::CameraSettings &camera)
{
    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    guiInternal.m_tracker.makeRoot(camera.m_size);
    guiInternal.m_shadow.flip();
}

void perfectpixel::core::GUI::pushHorizontalLayout(
    const AnchoredRegion &position, const LinearLayoutOptions &options)
{
    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    Anchor outlineRel;
    positionToSimpleRelative(position, outlineRel);

    guiInternal.m_tracker.addLayout(
        {outlineRel.m_left, outlineRel.m_bottom},
        {outlineRel.m_right, outlineRel.m_top},
        _internal::UILayoutTracker::LayoutDirection::HORIZONTAL,
        options.m_reverse ? -1.0f : 1.0f);
}

void perfectpixel::core::GUI::popLayout()
{
    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    guiInternal.m_tracker.pop();
}

void perfectpixel::core::GUI::positionToSimpleRelative(
    const AnchoredRegion &position, Anchor &outInner)
{
    Anchor temp;

    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    bedrock::Vector2 innerRel
        = guiInternal.m_tracker.absToRel(position.m_extend);

    temp.m_left   = innerRel.x * position.m_pivot.x;
    temp.m_right  = innerRel.x * (1.0f - position.m_pivot.x);
    temp.m_bottom = innerRel.y * position.m_pivot.y;
    temp.m_top    = innerRel.y * (1.0f - position.m_pivot.y);

    bedrock::Vector2 cursor = guiInternal.m_tracker.cursor();

    outInner.m_left   = position.m_anchor.m_left - temp.m_left + cursor.x;
    outInner.m_right  = position.m_anchor.m_right - temp.m_right + cursor.x;
    outInner.m_bottom = position.m_anchor.m_bottom - temp.m_bottom + cursor.y;
    outInner.m_top    = position.m_anchor.m_top - temp.m_top + cursor.y;
}
