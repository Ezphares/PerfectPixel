#pragma once

#include "bedrock/AARectangle.h"
#include "bedrock/Circle.h"
#include "bedrock/TypeReflection.h"

#include "ecs/Component.h"
#include "ecs/CoreComponentStorage.h"
#include "ecs/Entity.h"

#include <string>

namespace perfectpixel {
namespace ecs {
class EntityManager;
}
namespace serialization {
class BinarySerializer;
}
namespace physics {

enum ColliderMaskType : uint32_t
{
    CMT_UNSET,
    CMT_AA_RECTANGLE,
    CMT_CIRCLE
};
}
} // namespace perfectpixel
PP_TYPE_REFLECTION(::perfectpixel::physics::ColliderMaskType, ColliderMaskType);
namespace perfectpixel { namespace physics {

class AARectangleColliderMask
    : public ecs::Component<AARectangleColliderMask, ecs::MapComponentStorage>
{
public:
    PPField(AARectangleColliderMask, bedrock::Vector2, Center);
    PPField(AARectangleColliderMask, bedrock::Vector2, HalfSize);
};

class CircleColliderMask
    : public ecs::Component<CircleColliderMask, ecs::MapComponentStorage>
{
public:
    PPField(CircleColliderMask, bedrock::Vector2, Center);
    PPField(CircleColliderMask, float, Radius);
};

class ColliderComponent
    : public ecs::Component<ColliderComponent, ecs::LinearScanComponentStorage>
{
public:
    static void ClearMask(ecs::Entity entity);

    static void
    SetMaskRectangle(ecs::Entity entity, const bedrock::AARectangle &rectangle);
    static bedrock::AARectangle GetMaskRectangle(ecs::Entity entity);

    static void
    SetMaskCircle(ecs::Entity entity, const bedrock::Circle &circle);
    static const bedrock::Circle GetMaskCircle(ecs::Entity entity);

    static void
    GetNear(std::vector<ecs::Entity> &toCheck, const bedrock::Vector2 &point);

public:
    PPField(ColliderComponent, ColliderMaskType, MaskType);
    PPField(ColliderComponent, std::string, EventTag);
};
}} // namespace perfectpixel::physics
