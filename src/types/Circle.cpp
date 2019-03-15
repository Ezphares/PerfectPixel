#include <types/Circle.h>

namespace perfectpixel {
	namespace types {

		Circle::Circle(PpFloat radius, Vector2 center /*= Vector2()*/)
			: m_radius(radius)
			, m_center(center)
		{

		}

		Circle::~Circle()
		{
		}

	}
}
