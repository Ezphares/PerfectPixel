#include "renderer/SpriteComponent.h"

namespace perfectpixel { namespace renderer {

void SpriteComponent::SetSprite(ecs::Entity entity, core::Resource &res)
{
    core::Resource &spr = SpriteComponent::SpriteResource(entity);
    // if (spr != res) TODO: Implement equality operators
    {
        spr                                 = res;
        SpriteComponent::SpriteData(entity) = *spr.get<core::Sprite>();
    }
}

void SpriteComponent::Update(ecs::Entity entity, float deltaT)
{
    float &fta      = FrameTimeAccumulator(entity);
    float frameTime = 1.0f / FPS(entity);
    fta += deltaT;
    if (fta > frameTime)
    {
        fta -= frameTime;
        CurrentFrame(entity)++;
    }
}

}} // namespace perfectpixel::renderer
