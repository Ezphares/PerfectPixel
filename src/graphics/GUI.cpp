#include "graphics/GUI.h"
#include "graphics/CameraSettings.h"
#include "graphics/UILayoutTracker.h"
#include "graphics/UIShadow.h"

namespace perfectpixel { namespace graphics { namespace _internal {

class GUIInternal
{
public:
    UILayoutTracker m_tracker;
    UIShadow m_shadow;
};

}}} // namespace perfectpixel::graphics::_internal

perfectpixel::graphics::GUI::HorizontalLayout::HorizontalLayout(
    const Position &position,
    LinearLayoutOptions options /*= LinearLayoutOptions()*/)
{
    GUI::pushHorizontalLayout(position, options);
}

void perfectpixel::graphics::GUI::begin(const CameraSettings &camera)
{
    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    guiInternal.m_tracker.makeRoot(camera.m_size);
    guiInternal.m_shadow.flip();
}

void perfectpixel::graphics::GUI::pushHorizontalLayout(
    const Position &position, const LinearLayoutOptions &options)
{
    (void)position;
    (void)options;

    //_internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    // guiInternal.m_tracker.addLayout() TODO;
}

void perfectpixel::graphics::GUI::popLayout()
{
    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    guiInternal.m_tracker.pop();
}

void perfectpixel::graphics::GUI::positionToSimpleRelative(
    const Position &position,
    Rectangle &outInner,
    bedrock::Vector2 &outTotalConsumption)
{
    Rectangle temp;

    rectToRelative(position.m_size, outInner);
    rectToRelative(position.m_margin, temp);

    // FIXME: Calculations
    (void)outTotalConsumption;
}

void perfectpixel::graphics::GUI::rectToRelative(
    const Rectangle &in, Rectangle &out)
{
    _internal::GUIInternal &guiInternal = *(getInstance()->m_internal);

    memcpy(&out, &in, sizeof(Rectangle));

    out.m_relative = static_cast<AxisFlags>(AXIS_HOR | AXIS_VER);

    bedrock::Vector2 transformMin = guiInternal.m_tracker.absToRel(in.m_min);
    bedrock::Vector2 transformMax = guiInternal.m_tracker.absToRel(in.m_max);

    if ((in.m_relative & AXIS_HOR) == 0)
    {
        out.m_min.x() = transformMin.x();
        out.m_max.x() = transformMax.x();
    }

    if ((in.m_relative & AXIS_VER) == 0)
    {
        out.m_min.y() = transformMin.y();
        out.m_max.y() = transformMax.y();
    }
}

perfectpixel::graphics::GUI::Rectangle::Rectangle(
    bedrock::Vector2 start,
    bedrock::Vector2 end,
    AxisFlags relative /*= AXIS_NONE*/)
    : m_min(start)
    , m_max(end)
    , m_relative(relative)
{}

perfectpixel::graphics::GUI::Rectangle::Rectangle(
    bedrock::Vector2 size, AxisFlags relative /*= AXIS_NONE*/)
    : GUI::Rectangle(size, bedrock::Vector2(), relative)
{}

perfectpixel::graphics::GUI::Position::Position(
    Rectangle size /*= Rectangle()*/, Rectangle margin /*= Rectangle()*/)
    : m_size(size)
    , m_margin(margin)
{}
