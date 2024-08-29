#pragma once

#include "bedrock/vectors.h"

namespace perfectpixel { namespace bedrock {

class AARectangle
{
public:
    AARectangle(Vector2 size = Vector2(), Vector2 center = Vector2());

public:
    Vector2 m_halfSize, m_center;
};

}} // namespace perfectpixel::bedrock
