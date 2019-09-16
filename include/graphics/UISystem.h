#pragma once

#include <graphics/CBFGFont.h>
#include <graphics/DrawQueue.h>
#include <graphics/GraphicsManager.h>
#include <graphics/UITextComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/System.h>
#include <EntityComponentSystem/TransformComponent.h>

// FIXME Very temporary

namespace perfectpixel { namespace graphics {

class UISystem : public ecs::System
{
    typedef ecs::QueryHelper<
        ecs::With<UITextComponent, ecs::TransformComponent>>
        UIQuery;

public:
    UISystem()
        : System(UIQuery::build())
    {
        m_onRender = &onRender;
    }

    static void
    onRender(const RangeLimit &begin, const RangeLimit &end, float deltaT);

    static IFont *m_font;
    static GraphicsManager *m_gm;
};

}} // namespace perfectpixel::graphics
