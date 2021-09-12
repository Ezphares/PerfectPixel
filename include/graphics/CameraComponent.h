#pragma once

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>
#include <EntityComponentSystem/FieldHelper.h>

#include <Bedrock/vectors.h>

namespace perfectpixel { namespace graphics {

class CameraComponent
    : public ecs::Component<CameraComponent, ecs::DefaultComponentStorage>
{
private:
public:
    PPField(CameraComponent, bedrock::Vector2, Viewport);
    PPField(CameraComponent, bedrock::ID, TargetFrameBuffer);
};
}} // namespace perfectpixel::graphics
