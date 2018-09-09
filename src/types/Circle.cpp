#include <types/Circle.h>

namespace perfectpixel {
	namespace types {

		Circle::Circle(PpFloat diameter, Vector2 center /*= Vector2()*/)
			: m_diameter(diameter)
			, m_center(center)
		{

		}

		Circle::~Circle()
		{
		}

	}
}
