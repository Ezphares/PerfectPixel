#pragma once

#include "bedrock/vectors.h"

namespace perfectpixel { namespace physics {

const static int32_t FORCE_ID_INTERNAL_START = 1 << 16;
const static int32_t FORCE_ID_GRAVITY        = FORCE_ID_INTERNAL_START + 1;
const static int32_t FORCE_ID_DRAG           = FORCE_ID_INTERNAL_START + 2;

struct Force
{
    int32_t m_identifier = -1;
    bedrock::Vector3 m_vector;
    bool m_relativeDirection; // Relative direction is compared to FORWARD
    bool m_relativeMagnitude;
    bool m_ignoreMass = false;

    /// Create a constant force representing gravity. Magnitude is in Units /
    /// Second^2
    inline static Force gravity(float magnitude = 1)
    {
        return Force{
            FORCE_ID_GRAVITY,
            bedrock::Vector3::DOWN * magnitude,
            false,
            false,
            true};
    }

    /// Create a constant force representing drag. Magnitude is in Units /
    /// Second^2. Progressive drag gets stronger the faster an object moves
    inline static Force drag(float magnitude = 1, bool progressive = true)
    {
        return Force{
            FORCE_ID_DRAG,
            bedrock::Vector3::FORWARD * magnitude,
            true,
            progressive,
            true};
    }
};

}} // namespace perfectpixel::physics

PP_REFLECT(::perfectpixel::physics::Force);
