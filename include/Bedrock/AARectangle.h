#pragma once

#include <Bedrock/vectors.h>

namespace perfectpixel { namespace bedrock {

class AARectangle
{
public:
    AARectangle(
        Vector2 size   = Vector2(),
        Vector2 center = Vector2(),
        bool halfSize  = false);

public:
    Vector2 m_halfSize, m_center;
};

}} // namespace perfectpixel::bedrock
