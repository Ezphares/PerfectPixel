#pragma once

#pragma once

#include "renderer/RendererInterface.h"

#include "core/SpriteComponent.h"

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
        ecs::With<SpriteComponent, ecs::TransformComponent>>
        SpriteQuery;

public:
    SpriteDrawSystem()
        : QuerySystem(SpriteQuery::build())
    {
        m_onRender = &onRender;
    }

    static void onRender(std::span<ecs::Entity> entities, float deltaT);

    static renderer::RendererInterface *m_gm;

    static renderer::ImageResource *getImageResource(void *resourcePtr);
};

} // namespace perfectpixel::core
