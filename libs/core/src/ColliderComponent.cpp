#include "core/ColliderComponent.h"

#include "ecs/EntityManager.h"
#include "ecs/LifecycleComponents.h"

namespace perfectpixel { namespace physics {

void ColliderComponent::ClearMask(ecs::Entity entity)
{
    switch (MaskType(entity))
    {
    case ColliderMaskType::CMT_AA_RECTANGLE:
        AARectangleColliderMask::Delete(entity);
        break;
    case ColliderMaskType::CMT_CIRCLE:
        CircleColliderMask::Delete(entity);
        break;
    default:
        break;
    }
}

void ColliderComponent::SetMaskRectangle(
    ecs::Entity entity, const bedrock::AARectangle &rectangle)
{
    if (ColliderComponent::MaskType(entity) != CMT_AA_RECTANGLE)
    {
        ClearMask(entity);
        ColliderComponent::MaskType(entity) = CMT_AA_RECTANGLE;
        AARectangleColliderMask::Register(entity);
    }
    AARectangleColliderMask::Center(entity)   = rectangle.m_center;
    AARectangleColliderMask::HalfSize(entity) = rectangle.m_halfSize;
}

bedrock::AARectangle ColliderComponent::GetMaskRectangle(ecs::Entity entity)
{
    bedrock::AARectangle result;
    result.m_center   = AARectangleColliderMask::Center(entity);
    result.m_halfSize = AARectangleColliderMask::HalfSize(entity);
    return result;
}

void ColliderComponent::SetMaskCircle(
    ecs::Entity entity, const bedrock::Circle &circle)
{
    if (ColliderComponent::MaskType(entity) != CMT_CIRCLE)
    {
        ClearMask(entity);
        ColliderComponent::MaskType(entity) = CMT_CIRCLE;
        CircleColliderMask::Register(entity);
    }
    CircleColliderMask::Center(entity) = circle.m_center;
    CircleColliderMask::Radius(entity) = circle.m_radius;
}

const perfectpixel::bedrock::Circle
ColliderComponent::GetMaskCircle(ecs::Entity entity)
{
    bedrock::Circle result = bedrock::Circle(
        CircleColliderMask::Radius(entity), CircleColliderMask::Center(entity));
    return result;
}

void ColliderComponent::GetNear(
    std::vector<ecs::Entity> &toCheck, const bedrock::Vector2 &point)
{
    (void)point; // TODO: Do broadphase collision here

    bedrock::BitSet mask = ecs::EntityManager::getInstance()->all();
    Filter(mask, ecs::ComponentStorageFilterType::WITH);
    ecs::InactiveComponent::Filter(
        mask, ecs::ComponentStorageFilterType::WITHOUT);
    ecs::EntityManager::getInstance()->expandMask(mask, &toCheck, nullptr);
}

}} // namespace perfectpixel::physics
