#pragma once

#pragma once

#include "renderer/RendererInterface.h"
#include "renderer/SpriteComponent.h"

#include "ecs/QueryHelper.h"
#include "ecs/QuerySystem.h"
#include "ecs/TransformComponent.h"

namespace renderer {
class RendererInterface;
}

namespace perfectpixel::core {

class SpriteDrawSystem : public ecs::QuerySystem
{
    typedef ecs::QueryHelper<
        ecs::With<renderer::SpriteComponent, ecs::TransformComponent>>
        SpriteQuery;

public:
    SpriteDrawSystem()
        : QuerySystem(SpriteQuery::build())
    {
        m_onRender = &onRender;
    }

    static void
    onRender(const RangeLimit &begin, const RangeLimit &end, float deltaT);

    static renderer::RendererInterface *m_gm;
};

} // namespace perfectpixel::core
