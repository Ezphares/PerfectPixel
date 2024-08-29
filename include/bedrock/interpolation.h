#pragma once

namespace perfectpixel { namespace bedrock {

template <typename T>
T lerp(T a, T b, float alpha)
{
    return a * (1.0 - alpha) + b * alpha;
}

template <typename T>
float invLerp(T a, T b, T v)
{
    return (v - a) / (b - a);
}

}} // namespace perfectpixel::bedrock
