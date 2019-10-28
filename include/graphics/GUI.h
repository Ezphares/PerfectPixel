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

    struct Rectangle
    {
        Rectangle(
            bedrock::Vector2 size = bedrock::Vector2(),
            AxisFlags relative    = AXIS_NONE);
        Rectangle(
            bedrock::Vector2 start,
            bedrock::Vector2 end,
            AxisFlags relative = AXIS_NONE);

        bedrock::Vector2 m_min, m_max;
        AxisFlags m_relative;
    };

    struct Position
    {
        Position(Rectangle size = Rectangle(), Rectangle margin = Rectangle());

        Rectangle m_size, m_margin;
    };

    struct LinearLayoutOptions
    {};

    // RAII Layout system
    class HorizontalLayout
    {
        HorizontalLayout(
            const Position &position,
            LinearLayoutOptions options = LinearLayoutOptions());
        ~HorizontalLayout();

    private:
        HorizontalLayout(const HorizontalLayout &)  = delete;
        HorizontalLayout(const HorizontalLayout &&) = delete;
        HorizontalLayout operator=(const HorizontalLayout &) = delete;
        HorizontalLayout operator=(const HorizontalLayout &&) = delete;
    };

public:
    static void begin(const CameraSettings &camera);
    static void pushHorizontalLayout(
        const Position &position, const LinearLayoutOptions &options);
    static void popLayout();

private:
    static void positionToSimpleRelative(
        const Position &position,
        Rectangle &outInner,
        bedrock::Vector2 &outTotalConsumption);

    static void rectToRelative(const Rectangle &in, Rectangle &out);

private:
    _internal::GUIInternal *m_internal;
}; // namespace bedrock::Singleton<GUI>

}} // namespace perfectpixel::graphics
