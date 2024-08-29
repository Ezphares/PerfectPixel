#pragma once

#include "core/UI/UITextComponent.h"

#include "renderer/CBFGFont.h"
#include "renderer/DrawQueue.h"
#include "renderer/RendererInterface.h"

#include "ecs/QueryHelper.h"
#include "ecs/QuerySystem.h"
#include "ecs/TransformComponent.h"

// FIXME Very temporary

namespace perfectpixel::core {

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

    static void onRender(std::span<ecs::Entity> entities, float deltaT);

    static renderer::IFont *m_font;
    static renderer::RendererInterface *m_gm;
};

} // namespace perfectpixel::core
