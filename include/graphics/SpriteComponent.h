#pragma once

#include <graphics/RenderHint.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <Bedrock/vectors.h>

#include <Resources/Resource.h>
#include <Resources/Sprite.h>

namespace perfectpixel { namespace graphics {

class SpriteComponent
    : public ecs::Component<SpriteComponent, ecs::DefaultComponentStorage>
{
private:
public:
    PPResourceField(
        SpriteComponent, bedrock::typeID<resources::Sprite>(), SpriteResource);
    PPField(SpriteComponent, resources::Sprite, SpriteData);
    PPField(SpriteComponent, bedrock::Vector2, Size);
    PPField(SpriteComponent, bedrock::Vector2, Offset);
    PPField(SpriteComponent, float, FPS);
    PPField(
        SpriteComponent, uint32_t, DrawHint); // FIXME Renderhints Enum instead
    PPField(SpriteComponent, float, FrameTimeAccumulator);
    PPField(SpriteComponent, int32_t, CurrentFrame);

    PPDoNotSerialize(SpriteComponent, FrameTimeAccumulator);

    static void SetSprite(ecs::Entity entity, resources::Resource &res);

    static void Update(ecs::Entity entity, float deltaT);
};
}} // namespace perfectpixel::graphics
