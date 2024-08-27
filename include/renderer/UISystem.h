#pragma once

#include "renderer/CBFGFont.h"
#include "renderer/DrawQueue.h"
#include "renderer/RendererInterface.h"
#include "renderer/UITextComponent.h"

#include "ecs/QueryHelper.h"
#include "ecs/QuerySystem.h"
#include "ecs/TransformComponent.h"

// FIXME Very temporary

namespace perfectpixel { namespace renderer {

class UISystem : public ecs::QuerySystem
{
    typedef ecs::QueryHelper<
        ecs::With<UITextComponent, ecs::TransformComponent>>
        UIQuery;

public:
    UISystem()
        : QuerySystem(UIQuery::build())
    {
        m_onRender = &onRender;
    }

    static void
    onRender(const RangeLimit &begin, const RangeLimit &end, float deltaT);

    static IFont *m_font;
    static RendererInterface *m_gm;
};

}} // namespace perfectpixel::renderer
