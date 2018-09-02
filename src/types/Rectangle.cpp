#include <types/Rectangle.h>

namespace perfectpixel {
	namespace types {



		Rectangle::Rectangle(Vector2 size /*= Vector2()*/, Vector2 center /*= Vector2()*/)
			: m_size(size)
			, m_center(center)
		{

		}

		Rectangle::~Rectangle()
		{
		}

	}
}