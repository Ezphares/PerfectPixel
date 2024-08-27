#pragma once

#include "ecs/Component.h"
#include "ecs/CoreComponentStorage.h"
#include "ecs/FieldHelper.h"

#include "bedrock/vectors.h"

namespace perfectpixel { namespace renderer {

class CameraComponent
    : public ecs::Component<CameraComponent, ecs::DefaultComponentStorage>
{
private:
public:
    PPField(CameraComponent, bedrock::Vector2, Viewport);
    PPField(CameraComponent, bedrock::ID, TargetFrameBuffer);
};
}} // namespace perfectpixel::renderer
