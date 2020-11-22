#pragma once

#include <Bedrock/vectors.h>

namespace perfectpixel { namespace bedrock {

class Circle
{
public:
    Circle(float radius = 0.0f, Vector2 center = Vector2());

public:
    float m_radius;
    Vector2 m_center;
};

}} // namespace perfectpixel::bedrock
