#pragma once

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>
#include <EntityComponentSystem/EcsReflection.h>

#include <Bedrock/vectors.h>

namespace perfectpixel { namespace ecs {

class TransformComponent : public Component<
                               TransformComponent,
                               FlatComponentStorage<TransformComponent>>
{
public:
    PPField(TransformComponent, bedrock::Vector3, Position);
    // inline static Field<TransformComponent, types::Vector3> Position;
    // inline static Field<TransformComponent, types::Vector3> Velocity;
};

}} // namespace perfectpixel::ecs
