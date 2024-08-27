#include "bedrock/AARectangle.h"

namespace perfectpixel { namespace bedrock {

AARectangle::AARectangle(
    Vector2 size /*= Vector2()*/, Vector2 center /*= Vector2()*/)
    : m_halfSize(size)
    , m_center(center)
{}

}} // namespace perfectpixel::bedrock
