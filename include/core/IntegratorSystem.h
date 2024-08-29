#pragma once

#include "ecs/QuerySystem.h"

namespace perfectpixel { namespace physics {

class IntegratorSystem : public ecs::QuerySystem
{
public:
    IntegratorSystem();

    static void onUpdate(std::span<ecs::Entity> entities, float deltaT);
};

}} // namespace perfectpixel::physics
