#pragma once

#include <EntityComponentSystem/System.h>

namespace perfectpixel { namespace physics {

class IntegratorSystem : public ecs::System
{
public:
    IntegratorSystem();

    static void onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT);
};

}} // namespace perfectpixel::physics
