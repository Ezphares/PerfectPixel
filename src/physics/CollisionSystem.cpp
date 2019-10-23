#include <physics/ColliderComponent.h>
#include <physics/CollisionSystem.h>
#include <physics/PhysicsComponent.h>

#include <EntityComponentSystem/TransformComponent.h>

#include <Bedrock/Logger.h>

#include <cmath>
#include <set>

namespace perfectpixel { namespace physics {

uint32_t CollisionSystem::m_collisionCount = 0u;

namespace {
float COLLISON_LEEWAY = 0.01f;
typedef ecs::QueryHelper<ecs::With<ColliderComponent, ecs::TransformComponent>>
    CollisionQuery;
} // namespace

CollisionSystem::CollisionSystem()
    : QuerySystem(CollisionQuery::build())
{
    m_onUpdate = &onUpdate;
}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::onUpdate(
    const RangeLimit &begin, const RangeLimit &end, float deltaT)
{
    (void)deltaT;

    m_collisionCount = 0u;
    std::set<ecs::Entity> collisionCache;

    for (auto it = begin; it != end; ++it)
    {
        collisionCache.insert(*it);
        collideSingle(*it, collisionCache);
    }
}

uint32_t CollisionSystem::getCollisionsLastUpdate() { return m_collisionCount; }

void CollisionSystem::collideSingle(
    ecs::Entity entity, std::set<ecs::Entity> &cache)
{
    std::vector<ecs::Entity> toCheck;
    possibleCollisions(entity, cache, toCheck);

    // Store mass as we might change it for resolving chained collisions
    float storeMass
        = PhysicsComponent::Has(entity) ? PhysicsComponent::Mass(entity) : 0.0f;

    for (ecs::Entity other : toCheck)
    {
        CollisionData collision;
        if (checkCollision(entity, other, collision))
        {
            ++m_collisionCount;
            resolveCollision(collision);
        }
    }

    if (PhysicsComponent::Has(entity))
    {
        PhysicsComponent::Mass(entity) = storeMass;
    }
}

void CollisionSystem::possibleCollisions(
    const ecs::Entity entity,
    std::set<ecs::Entity> &cache,
    std::vector<ecs::Entity> &out_possibleCollisions)
{
    std::vector<ecs::Entity> toCheck;
    ColliderComponent::GetNear(toCheck, absoluteCenter(entity));

    for (auto other : toCheck)
    {
        if (cache.find(other) == cache.end())
        {
            out_possibleCollisions.push_back(other);
        }
    }
}

bool CollisionSystem::checkCollision(
    ecs::Entity first, ecs::Entity second, CollisionData &out_collision)
{
    out_collision.m_first  = first;
    out_collision.m_second = second;

    if (ColliderComponent::MaskType(first) == CMT_AA_RECTANGLE
        && ColliderComponent::MaskType(second) == CMT_AA_RECTANGLE)
    {
        return collideRectRect(
            first,
            ColliderComponent::GetMaskRectangle(first),
            second,
            ColliderComponent::GetMaskRectangle(second),
            out_collision);
    }
    else if (
        ColliderComponent::MaskType(first) == CMT_CIRCLE
        && ColliderComponent::MaskType(second) == CMT_CIRCLE)
    {
        return collideCircleCircle(
            first,
            ColliderComponent::GetMaskCircle(first),
            second,
            ColliderComponent::GetMaskCircle(second),
            out_collision);
    }

    // TODO Call other mask types
    return false;
}

bool CollisionSystem::collideRectRect(
    ecs::Entity first,
    const bedrock::AARectangle &firstRect,
    ecs::Entity second,
    const bedrock::AARectangle &secondRect,
    CollisionData &out_collision)
{
    bedrock::Vector2 firstPosition
        = bedrock::Vector2(ecs::TransformComponent::Position(first)),
        secondPosition
        = bedrock::Vector2(ecs::TransformComponent::Position(second));

    bedrock::Vector2 offset = (secondPosition + secondRect.m_center)
                              - (firstPosition + firstRect.m_center);

    bedrock::Vector2 overlap{
        firstRect.m_halfSize.x() + secondRect.m_halfSize.x()
            - std::abs(offset.x()),
        firstRect.m_halfSize.y() + secondRect.m_halfSize.y()
            - std::abs(offset.y())};

    if (overlap.x() < COLLISON_LEEWAY || overlap.y() < COLLISON_LEEWAY)
    {
        // No collision
        return false;
    }

    out_collision.m_data_RectRectOverlap = overlap;
    out_collision.m_maskTypeFirst
        = CMT_AA_RECTANGLE;
    out_collision.m_maskTypeSecond
        = CMT_AA_RECTANGLE;

    return true;
}

bool CollisionSystem::collideCircleCircle(
    ecs::Entity first,
    const bedrock::Circle &firstCircle,
    ecs::Entity second,
    const bedrock::Circle &secondCircle,
    CollisionData &out_collision)
{
    bedrock::Vector2 firstPosition
        = bedrock::Vector2(ecs::TransformComponent::Position(first)),
        secondPosition
        = bedrock::Vector2(ecs::TransformComponent::Position(second));

    bedrock::Vector2 offset = (secondPosition + secondCircle.m_center)
                              - (firstPosition + firstCircle.m_center);
    float squareDistance = bedrock::Vector2::dot(offset, offset);
    float sumRadii       = firstCircle.m_radius + secondCircle.m_radius;

    if (squareDistance
        > (sumRadii - COLLISON_LEEWAY) * (sumRadii - COLLISON_LEEWAY))
    {
        // No collision
        return false;
    }

    out_collision.m_data_CircCircOverlap = sumRadii - std::sqrt(squareDistance);
    out_collision.m_maskTypeFirst = ColliderMaskType::CMT_CIRCLE;
    out_collision.m_maskTypeSecond
        = ColliderMaskType::CMT_CIRCLE;

    return true;
}

void CollisionSystem::resolveCollision(const CollisionData &collision)
{
    // FIXME: Generate events

    if (!PhysicsComponent::Has(collision.m_first)
        || !PhysicsComponent::Has(collision.m_second))
    {
        // Without physics, resolution is done
        return;
    }

    bedrock::Vector2 resolution1{0, 0}, resolution2{0, 0};
    bedrock::Vector2 bounce1{
        ecs::TransformComponent::Velocity(collision.m_first)},
        bounce2{ecs::TransformComponent::Velocity(collision.m_second)};

    float bounciness = std::max(
        PhysicsComponent::Bounciness(collision.m_first),
        PhysicsComponent::Bounciness(collision.m_second));

    if (collision.m_maskTypeFirst
            == ColliderMaskType::CMT_AA_RECTANGLE
        && collision.m_maskTypeSecond
               == ColliderMaskType::CMT_AA_RECTANGLE)
    {
        bedrock::Vector2 overlap = collision.m_data_RectRectOverlap;
        float newVel1, newVel2;

        if (overlap.x() < overlap.y())
        {
            singleAxisReposition(
                PhysicsComponent::Mass(collision.m_first),
                PhysicsComponent::Mass(collision.m_second),
                overlap.x(),
                &(resolution1.m_data[0]),
                &(resolution2.m_data[0]));

            if (absoluteCenter(collision.m_first).x()
                > absoluteCenter(collision.m_second).x())
            {
                resolution2 *= -1;
            }
            else
            {
                resolution1 *= -1;
            }

            singleAxisBounce(
                bounciness,
                PhysicsComponent::Mass(collision.m_first),
                PhysicsComponent::Mass(collision.m_second),
                ecs::TransformComponent::Velocity(collision.m_first).x(),
                ecs::TransformComponent::Velocity(collision.m_second).x(),
                &newVel1,
                &newVel2);

            bounce1
                = {newVel1,
                   ecs::TransformComponent::Velocity(collision.m_first).y()};
            bounce2
                = {newVel2,
                   ecs::TransformComponent::Velocity(collision.m_first).y()};
        }
        else
        {
            singleAxisReposition(
                PhysicsComponent::Mass(collision.m_first),
                PhysicsComponent::Mass(collision.m_second),
                overlap.y(),
                &(resolution1.m_data[1]),
                &(resolution2.m_data[1]));
            if (absoluteCenter(collision.m_first).y()
                > absoluteCenter(collision.m_second).y())
            {
                resolution2 *= -1;
            }
            else
            {
                resolution1 *= -1;
            }

            singleAxisBounce(
                bounciness,
                PhysicsComponent::Mass(collision.m_first),
                PhysicsComponent::Mass(collision.m_second),
                ecs::TransformComponent::Velocity(collision.m_first).y(),
                ecs::TransformComponent::Velocity(collision.m_second).y(),
                &newVel1,
                &newVel2);

            bounce1 = {ecs::TransformComponent::Velocity(collision.m_first).x(),
                       newVel1};
            bounce2
                = {ecs::TransformComponent::Velocity(collision.m_second).x(),
                   newVel2};
        }
    }
    else if (
        collision.m_maskTypeFirst == ColliderMaskType::CMT_CIRCLE
        && collision.m_maskTypeSecond
               == ColliderMaskType::CMT_CIRCLE)
    {
        bedrock::Vector2 resolutionAxis = (absoluteCenter(collision.m_second)
                                           - absoluteCenter(collision.m_first))
                                              .normal();

        float magnitude1, magnitude2;

        singleAxisReposition(
            PhysicsComponent::Mass(collision.m_first),
            PhysicsComponent::Mass(collision.m_second),
            collision.m_data_CircCircOverlap,
            &magnitude1,
            &magnitude2);

        resolution1 = resolutionAxis * -magnitude1;
        resolution2 = resolutionAxis * magnitude2;

        // FIXME bounce
    }
    else
    {
        // Unsupported collision type
    }

    ecs::TransformComponent::Position(collision.m_first)
        += bedrock::Vector3(resolution1);
    ecs::TransformComponent::Position(collision.m_second)
        += bedrock::Vector3(resolution2);

    ecs::TransformComponent::Velocity(collision.m_first)
        = bedrock::Vector3(bounce1);
    ecs::TransformComponent::Velocity(collision.m_second)
        = bedrock::Vector3(bounce2);
}

void CollisionSystem::singleAxisReposition(
    float mass1,
    float mass2,
    float overlap,
    float *out_magnitude1,
    float *out_magnitude2)
{
    *out_magnitude1 = 0;
    *out_magnitude2 = 0;

    // Special cases:
    if ((mass1 == 0 && mass2 == 0) || // Zero mass can never affect others
        (mass1 == bedrock::Infinity
         && mass2 == bedrock::Infinity)) // Infinite mass can never be affected
    {
        return;
    }

    // Get rid of infinities for math, we can be sure only one of them is
    // infinity given above sanity check
    if (mass1 == bedrock::Infinity)
    {
        mass1 = 1;
        mass2 = 0;
    }
    else if (mass2 == bedrock::Infinity)
    {
        mass1 = 0;
        mass2 = 1;
    }

    *out_magnitude1 = mass2 / (mass1 + mass2) * overlap;
    *out_magnitude2 = mass1 / (mass1 + mass2) * overlap;
}

void CollisionSystem::singleAxisBounce(
    float bounciness,
    float mass1,
    float mass2,
    float velocity1,
    float velocity2,
    float *out_newVelocity1,
    float *out_newVelocity2)
{
    *out_newVelocity1 = velocity1;
    *out_newVelocity2 = velocity2;

    // Special cases:
    if ((mass1 == 0 && mass2 == 0) || // Zero mass can never affect others
        (mass1 == bedrock::Infinity
         && mass2 == bedrock::Infinity)) // Infinite mass can never be affected
    {
        return;
    }

    // Get rid of infinities for math, we can be sure only one of them is
    // infinity given above sanity check
    if (mass1 == bedrock::Infinity)
    {
        mass1 = 1;
        mass2 = 0;
    }
    else if (mass2 == bedrock::Infinity)
    {
        mass1 = 0;
        mass2 = 1;
    }

    *out_newVelocity1 = (bounciness * mass2 * (velocity2 - velocity1)
                         + mass1 * velocity1 + mass2 * velocity2)
                        / (mass1 + mass2);
    *out_newVelocity2 = (bounciness * mass1 * (velocity1 - velocity2)
                         + mass1 * velocity1 + mass2 * velocity2)
                        / (mass1 + mass2);
}

bedrock::Vector2 CollisionSystem::absoluteCenter(ecs::Entity entity)
{
    bedrock::Vector2 result
        = bedrock::Vector2(ecs::TransformComponent::Position(entity));

    switch (ColliderComponent::MaskType(entity))
    {
    case CMT_AA_RECTANGLE:
        result += AARectangleColliderMask::Center(entity);
        break;

    case CMT_CIRCLE:
        result += CircleColliderMask::Center(entity);
        break;

    default:
        break;
    }

    return result;
}

}} // namespace perfectpixel::physics
