#pragma once

#include "ecs/Component.h"
#include "ecs/CoreComponentStorage.h"
#include "ecs/ReflectionTable.h"

#include "bedrock/vectors.h"

#ifndef TRANSFORM_COMPONENT_CAPACITY
#define TRANSFORM_COMPONENT_CAPACITY 4096
#endif

namespace perfectpixel { namespace ecs {

class TransformComponent
    : public Component<TransformComponent, FlatComponentStorage>
{
public:
    PPField(TransformComponent, bedrock::Vector3, Position);
    // inline static Field<TransformComponent, types::Vector3> Position;
    // inline static Field<TransformComponent, types::Vector3> Velocity;
};

}} // namespace perfectpixel::ecs
