#include "types/Line.h"

namespace perfectpixel { namespace types {

Line2D::Line2D(
    Vector2 direction,
    Vector2 referencePoint,
    PpFloat halfLength /*= Infinity*/)
    : m_directionVector(direction.normal())
    , m_referencePoint(referencePoint)
    , m_halfLength(halfLength)
{}

Line2D::~Line2D()
{}

}} // namespace perfectpixel::types
