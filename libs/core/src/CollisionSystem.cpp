#include "core/ColliderComponent.h"
#include "core/CollisionSystem.h"
#include "core/PhysicsComponent.h"

#include "ecs/TransformComponent.h"

#include "bedrock/Logger.h"

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

CollisionSystem::~CollisionSystem()
{}

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

uint32_t CollisionSystem::getCollisionsLastUpdate()
{
    return m_collisionCount;
}

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
            createProxyAARectangle(first),
            createProxyAARectangle(second),
            out_collision);
    }
    else if (
        ColliderComponent::MaskType(first) == CMT_CIRCLE
        && ColliderComponent::MaskType(second) == CMT_CIRCLE)
    {
        return collideCircleCircle(
            createProxyCircle(first), createProxyCircle(second), out_collision);
    }
    else if (
        ColliderComponent::MaskType(first) == CMT_AA_RECTANGLE
        && ColliderComponent::MaskType(second) == CMT_CIRCLE)
    {
        return collideRectCircle(
            createProxyAARectangle(first),
            createProxyCircle(second),
            out_collision);
    }
    else if (
        ColliderComponent::MaskType(first) == CMT_CIRCLE
        && ColliderComponent::MaskType(second) == CMT_AA_RECTANGLE)
    {
        std::swap(out_collision.m_first, out_collision.m_second);
        return collideRectCircle(
            createProxyAARectangle(second),
            createProxyCircle(first),
            out_collision);
    }

    // TODO Call other mask types
    return false;
}

bool CollisionSystem::collideRectRect(
    const bedrock::AARectangle &firstRect,
    const bedrock::AARectangle &secondRect,
    CollisionData &out_collision)
{
    bedrock::Vector2 offset = secondRect.m_center - firstRect.m_center;

    bedrock::Vector2 overlap{
        firstRect.m_halfSize.x + secondRect.m_halfSize.x - std::abs(offset.x),
        firstRect.m_halfSize.y + secondRect.m_halfSize.y - std::abs(offset.y)};

    if (overlap.x < COLLISON_LEEWAY || overlap.y < COLLISON_LEEWAY)
    {
        // No collision
        return false;
    }

    out_collision.m_data_RectRectOverlap = overlap;
    out_collision.m_maskTypeFirst        = CMT_AA_RECTANGLE;
    out_collision.m_maskTypeSecond       = CMT_AA_RECTANGLE;
    out_collision.m_firstProxy.m_aaRect  = firstRect;
    out_collision.m_secondProxy.m_aaRect = secondRect;

    return true;
}

bool CollisionSystem::collideCircleCircle(
    const bedrock::Circle &firstCircle,
    const bedrock::Circle &secondCircle,
    CollisionData &out_collision)
{
    bedrock::Vector2 offset = secondCircle.m_center - firstCircle.m_center;
    float squareDistance    = bedrock::Vector2::dot(offset, offset);
    float sumRadii          = firstCircle.m_radius + secondCircle.m_radius;

    if (squareDistance
        > (sumRadii - COLLISON_LEEWAY) * (sumRadii - COLLISON_LEEWAY))
    {
        // No collision
        return false;
    }

    out_collision.m_data_CircCircOverlap = sumRadii - std::sqrt(squareDistance);
    out_collision.m_maskTypeFirst        = ColliderMaskType::CMT_CIRCLE;
    out_collision.m_maskTypeSecond       = ColliderMaskType::CMT_CIRCLE;
    out_collision.m_firstProxy.m_circle  = firstCircle;
    out_collision.m_secondProxy.m_circle = secondCircle;

    return true;
}

bool CollisionSystem::collideRectCircle(
    const bedrock::AARectangle &firstRect,
    const bedrock::Circle &secondCircle,
    CollisionData &out_collision)
{
    // AABB overlap early out
    bedrock::AARectangle circleToRect;
    circleToRect.m_center = secondCircle.m_center;
    circleToRect.m_halfSize
        = bedrock::Vector2(secondCircle.m_radius, secondCircle.m_radius);
    if (!collideRectRect(firstRect, circleToRect, out_collision))
    {
        return false;
    }

    // First check if the circle collides fully with the rectangle part of the
    // AABB, in that case we can resolve as rectangle

    bool rectCheck          = false;
    float rectOverlapX      = bedrock::Infinity;
    float rectOverlapY      = bedrock::Infinity;
    circleToRect.m_halfSize = bedrock::Vector2(secondCircle.m_radius, 0.0f);
    if (collideRectRect(firstRect, circleToRect, out_collision))
    {
        rectOverlapX = out_collision.m_data_RectRectOverlap.x;
        rectCheck    = true;
    }

    circleToRect.m_halfSize = bedrock::Vector2(0.0f, secondCircle.m_radius);
    if (collideRectRect(firstRect, circleToRect, out_collision))
    {
        rectOverlapY = out_collision.m_data_RectRectOverlap.y;
        rectCheck    = true;
    }

    if (rectCheck)
    {
        out_collision.m_data_RectRectOverlap = {rectOverlapX, rectOverlapY};
        return true;
    }

    // Otherwise check a circle collision against the nearest corner
    bedrock::Circle rectToCircle;
    rectToCircle.m_center.x = firstRect.m_center.x
                              + (firstRect.m_center.x > secondCircle.m_center.x
                                     ? -firstRect.m_halfSize.x
                                     : firstRect.m_halfSize.x);
    rectToCircle.m_center.y = firstRect.m_center.y
                              + (firstRect.m_center.y > secondCircle.m_center.y
                                     ? -firstRect.m_halfSize.y
                                     : firstRect.m_halfSize.y);

    return collideCircleCircle(rectToCircle, secondCircle, out_collision);
}

perfectpixel::bedrock::AARectangle
CollisionSystem::createProxyAARectangle(ecs::Entity entity)
{
    bedrock::AARectangle result = ColliderComponent::GetMaskRectangle(entity);
    result.m_center
        += bedrock::Vector2(ecs::TransformComponent::Position(entity));

    return result;
}

perfectpixel::bedrock::Circle
CollisionSystem::createProxyCircle(ecs::Entity entity)
{
    bedrock::Circle result = ColliderComponent::GetMaskCircle(entity);
    result.m_center
        += bedrock::Vector2(ecs::TransformComponent::Position(entity));

    return result;
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
    bedrock::Vector2 bounce1{PhysicsComponent::Velocity(collision.m_first)},
        bounce2{PhysicsComponent::Velocity(collision.m_second)};

    float bounciness = std::max(
        PhysicsComponent::Bounciness(collision.m_first),
        PhysicsComponent::Bounciness(collision.m_second));

    if (collision.m_maskTypeFirst == ColliderMaskType::CMT_AA_RECTANGLE
        && collision.m_maskTypeSecond == ColliderMaskType::CMT_AA_RECTANGLE)
    {
        bedrock::Vector2 overlap = collision.m_data_RectRectOverlap;
        float newVel1, newVel2;

        if (overlap.x < overlap.y)
        {
            singleAxisReposition(
                PhysicsComponent::Mass(collision.m_first),
                PhysicsComponent::Mass(collision.m_second),
                overlap.x,
                &(resolution1[0]),
                &(resolution2[0]));

            if (collision.m_firstProxy.m_aaRect.m_center.x
                > collision.m_secondProxy.m_aaRect.m_center.x)
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
                PhysicsComponent::Velocity(collision.m_first).x,
                PhysicsComponent::Velocity(collision.m_second).x,
                &newVel1,
                &newVel2);

            bounce1
                = {newVel1, PhysicsComponent::Velocity(collision.m_first).y};
            bounce2
                = {newVel2, PhysicsComponent::Velocity(collision.m_first).y};
        }
        else
        {
            singleAxisReposition(
                PhysicsComponent::Mass(collision.m_first),
                PhysicsComponent::Mass(collision.m_second),
                overlap.y,
                &(resolution1[1]),
                &(resolution2[1]));
            if (collision.m_firstProxy.m_aaRect.m_center.y
                > collision.m_secondProxy.m_aaRect.m_center.y)
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
                PhysicsComponent::Velocity(collision.m_first).y,
                PhysicsComponent::Velocity(collision.m_second).y,
                &newVel1,
                &newVel2);

            bounce1
                = {PhysicsComponent::Velocity(collision.m_first).x, newVel1};
            bounce2
                = {PhysicsComponent::Velocity(collision.m_second).x, newVel2};
        }
    }
    else if (
        collision.m_maskTypeFirst == ColliderMaskType::CMT_CIRCLE
        && collision.m_maskTypeSecond == ColliderMaskType::CMT_CIRCLE)
    {
        bedrock::Vector2 resolutionAxis
            = (collision.m_secondProxy.m_circle.m_center
               - collision.m_firstProxy.m_circle.m_center)
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

        const bedrock::Vector2 vel1
            = bedrock::Vector2(PhysicsComponent::Velocity(collision.m_first));
        const bedrock::Vector2 vel2
            = bedrock::Vector2(PhysicsComponent::Velocity(collision.m_second));

        const float dot1 = bedrock::Vector2::dot(vel1, resolutionAxis);
        const float dot2 = bedrock::Vector2::dot(vel2, resolutionAxis);

        float newVel1, newVel2;

        singleAxisBounce(
            bounciness,
            PhysicsComponent::Mass(collision.m_first),
            PhysicsComponent::Mass(collision.m_second),
            dot1,
            dot2,
            &newVel1,
            &newVel2);

        const bedrock::Vector2 projection1 = resolutionAxis * dot1;
        const bedrock::Vector2 projection2 = resolutionAxis * dot2;
        const bedrock::Vector2 rejection1  = vel1 - projection1;
        const bedrock::Vector2 rejection2  = vel2 - projection2;

        bounce1 = rejection1 + projection1 * newVel1;
        bounce2 = rejection2 + projection2 * newVel2;
    }
    else
    {
        // Unsupported collision type
    }

    ecs::TransformComponent::Position(collision.m_first)
        += bedrock::Vector3(resolution1);
    ecs::TransformComponent::Position(collision.m_second)
        += bedrock::Vector3(resolution2);

    PhysicsComponent::Velocity(collision.m_first)  = bedrock::Vector3(bounce1);
    PhysicsComponent::Velocity(collision.m_second) = bedrock::Vector3(bounce2);
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
