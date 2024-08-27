#pragma once

#include "ecs/QuerySystem.h"

namespace perfectpixel { namespace physics {

class IntegratorSystem : public ecs::QuerySystem
{
public:
    IntegratorSystem();

    static void onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT);
};

}} // namespace perfectpixel::physics
