#pragma once

#include <physics/Force.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <EntityComponentSystem/Entity.h>

namespace perfectpixel { namespace physics {

enum PhysicsSimulationType : uint32_t
{
    PST_TRIGGER,
    PST_KINEMATIC,
    PST_FULL
};
}} // namespace perfectpixel::physics

PP_TYPE_REFLECTION(
    ::perfectpixel::physics::PhysicsSimulationType, PhysicsSimulationType);

namespace perfectpixel { namespace physics {

class PhysicsComponent : public ecs::Component<PhysicsComponent>,
                         public ecs::LinearScanComponentStorage
{
public:
    PPField(PhysicsComponent, float, Mass);
    PPField(PhysicsComponent, float, Bounciness);
    PPField(PhysicsComponent, PhysicsSimulationType, SimulationType);
    PPField(PhysicsComponent, bedrock::Vector3, Velocity);
    PPArrayField(PhysicsComponent, physics::Force, 8u, ActiveForces);

    static void MakeStaticCollider(ecs::Entity entity);
    static void ActivateForce(ecs::Entity entity, const Force &force);
    static void DeactivateForce(ecs::Entity entity, int32_t forceId);
};
}} // namespace perfectpixel::physics
