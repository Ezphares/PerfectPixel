#pragma once

#include "renderer/RenderHint.h"

#include "ecs/Component.h"
#include "ecs/CoreComponentStorage.h"

#include "bedrock/vectors.h"

#include "core/Resource.h"
#include "core/Sprite.h"

namespace perfectpixel::core {

class SpriteComponent
    : public ecs::Component<SpriteComponent, ecs::DefaultComponentStorage>
{
private:
public:
    PPResourceField(
        SpriteComponent, bedrock::typeID<core::Sprite>(), SpriteResource);
    PPField(SpriteComponent, core::Sprite, SpriteData);
    PPField(SpriteComponent, bedrock::Vector2, Size);
    PPField(SpriteComponent, bedrock::Vector2, Offset);
    PPField(SpriteComponent, float, FPS);
    PPField(
        SpriteComponent, uint32_t, DrawHint); // FIXME Renderhints Enum instead
    PPField(SpriteComponent, float, FrameTimeAccumulator);
    PPField(SpriteComponent, int32_t, CurrentFrame);

    PPDoNotSerialize(SpriteComponent, FrameTimeAccumulator);

    static void SetSprite(ecs::Entity entity, core::Resource &res);

    static void Update(ecs::Entity entity, float deltaT);
};
} // namespace perfectpixel::core
