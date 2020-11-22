#pragma once

#include <physics/ColliderComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/QuerySystem.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <EntityComponentSystem/Entity.h>

#include <set>
#include <vector>

namespace perfectpixel { namespace physics {

class CollisionSystem : public ecs::QuerySystem
{
public:
    struct ProxyShape
    {
        inline ProxyShape(){};

        union
        {
            bedrock::Circle m_circle;
            bedrock::AARectangle m_aaRect;
        };
    };

    struct CollisionData
    {
        inline CollisionData(){};
        ecs::Entity m_first, m_second;
        ColliderMaskType m_maskTypeFirst, m_maskTypeSecond;
        union
        {
            bedrock::Vector2 m_data_RectRectOverlap;
            float m_data_CircCircOverlap;
        };

        ProxyShape m_firstProxy, m_secondProxy;
    };

public:
    CollisionSystem();
    virtual ~CollisionSystem();

    static void
    onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT);

    // Resolution utilities
    static void singleAxisReposition(
        float mass1,
        float mass2,
        float overlap,
        float *out_magnitude1,
        float *out_magnitude2);
    static void singleAxisBounce(
        float bounciness,
        float mass1,
        float mass2,
        float velocity1,
        float velocity2,
        float *out_newVelocity1,
        float *out_newVelocity2);

    static uint32_t getCollisionsLastUpdate();

public:
    static void collideSingle(ecs::Entity entity, std::set<ecs::Entity> &cache);
    static void possibleCollisions(
        const ecs::Entity entity,
        std::set<ecs::Entity> &cache,
        std::vector<ecs::Entity> &out_possibleCollisions);

    static bool checkCollision(
        ecs::Entity first, ecs::Entity second, CollisionData &out_collision);
    static bool collideRectRect(
        const bedrock::AARectangle &firstRect,
        const bedrock::AARectangle &secondRect,
        CollisionData &out_collision);
    static bool collideCircleCircle(
        const bedrock::Circle &firstCircle,
        const bedrock::Circle &secondCircle,
        CollisionData &out_collision);
    static bool collideRectCircle(
        const bedrock::AARectangle &firstRect,
        const bedrock::Circle &secondCircle,
        CollisionData &out_collision);

    static bedrock::AARectangle createProxyAARectangle(ecs::Entity entity);
    static bedrock::Circle createProxyCircle(ecs::Entity entity);

    static void resolveCollision(const CollisionData &collision);

    static bedrock::Vector2 absoluteCenter(ecs::Entity entity);

private:
    static uint32_t m_collisionCount;
};

}} // namespace perfectpixel::physics
