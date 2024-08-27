#pragma once

#include "bedrock/Singleton.h"
#include "bedrock/vectors.h"

#include "core/Sprite.h"

namespace perfectpixel { namespace renderer {

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
            : m_left(point.x)
            , m_right(point.x)
            , m_bottom(point.y)
            , m_top(point.y)
        {}

        float m_left, m_right, m_bottom, m_top;
    };

    struct AnchoredRegion
    {
        AnchoredRegion()
            : m_anchor()
            , m_pivot()
            , m_extend()
        {}

        static AnchoredRegion absolute(
            bedrock::Vector2 absSize,
            PositionHelper position
            = static_cast<PositionHelper>(ALIGN_LEFT | ALIGN_TOP));

        Anchor m_anchor;
        bedrock::Vector2 m_pivot;
        bedrock::Vector2 m_extend;
    };

    struct NineSlice
    {
        Anchor m_sprite, m_target;
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
            const AnchoredRegion &position,
            LinearLayoutOptions options = LinearLayoutOptions());
        ~HorizontalLayout();

    private:
        HorizontalLayout(const HorizontalLayout &)            = delete;
        HorizontalLayout(const HorizontalLayout &&)           = delete;
        HorizontalLayout operator=(const HorizontalLayout &)  = delete;
        HorizontalLayout operator=(const HorizontalLayout &&) = delete;
    };

    // Primitives
    static void Spacer(float size, bool relative = false);
    static void DrawSprite(
        const core::Sprite &sprite,
        int32_t frame,
        const AnchoredRegion &region);
    static void DrawSprite(
        const core::Sprite &sprite,
        int32_t frame,
        const AnchoredRegion &region,
        const NineSlice &nineSlice);
    /* TODO fonts need to be moved to resources
    static void DrawText(
        const resources::Font &font,
        const std::string &text,
        const AnchoredRegion &position);
        */

    static void begin(const CameraSettings &camera);

private:
    // Layout internals
    static void pushHorizontalLayout(
        const AnchoredRegion &position, const LinearLayoutOptions &options);
    static void popLayout();

private:
    static void
    positionToSimpleRelative(const AnchoredRegion &position, Anchor &outInner);

private:
    _internal::GUIInternal *m_internal;
}; // namespace bedrock::Singleton<GUI>

}} // namespace perfectpixel::renderer
