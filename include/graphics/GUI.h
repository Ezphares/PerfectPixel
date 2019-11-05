#pragma once

#include "Bedrock/Singleton.h"
#include "Bedrock/vectors.h"

namespace perfectpixel { namespace graphics {

struct CameraSettings;
namespace _internal {
class GUIInternal;
}
class GUI : public bedrock::Singleton<GUI>
{
public:
    enum AxisFlags
    {
        AXIS_NONE = 0x0,
        AXIS_HOR  = 0x1,
        AXIS_VER  = 0x2,
    };

    enum PositionHelper
    {
        ALIGN_TOP    = 0x1,
        ALIGN_BOTTOM = 0x2,
        ALIGN_LEFT   = 0x4,
        ALIGN_RIGHT  = 0x8,
    };

    struct Anchor
    {
        Anchor(bedrock::Vector2 point = bedrock::Vector2())
            : m_left(point.x())
            , m_right(point.x())
            , m_bottom(point.y())
            , m_top(point.y())
        {}

        float m_left, m_right, m_bottom, m_top;
    };

    struct Rectangle
    {
        Rectangle()
            : m_anchor()
            , m_pivot()
            , m_extend()
        {}

        static Rectangle absolute(
            bedrock::Vector2 absSize,
            PositionHelper position
            = static_cast<PositionHelper>(ALIGN_LEFT | ALIGN_TOP));

        Anchor m_anchor;
        bedrock::Vector2 m_pivot;
        bedrock::Vector2 m_extend;
    };

    struct LinearLayoutOptions
    {
        LinearLayoutOptions()
            : m_reverse(false)
        {}
        bool m_reverse;
    };

    // RAII Layout system
    class HorizontalLayout
    {
        HorizontalLayout(
            const Rectangle &position,
            LinearLayoutOptions options = LinearLayoutOptions());
        ~HorizontalLayout();

    private:
        HorizontalLayout(const HorizontalLayout &)  = delete;
        HorizontalLayout(const HorizontalLayout &&) = delete;
        HorizontalLayout operator=(const HorizontalLayout &) = delete;
        HorizontalLayout operator=(const HorizontalLayout &&) = delete;
    };

    static void Spacer(float size, bool relative = false);

public:
    static void begin(const CameraSettings &camera);
    static void pushHorizontalLayout(
        const Rectangle &position, const LinearLayoutOptions &options);
    static void popLayout();

private:
    static void
    positionToSimpleRelative(const Rectangle &position, Anchor &outInner);

private:
    _internal::GUIInternal *m_internal;
}; // namespace bedrock::Singleton<GUI>

}} // namespace perfectpixel::graphics
