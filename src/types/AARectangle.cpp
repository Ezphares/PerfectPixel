#include <types/AARectangle.h>

namespace perfectpixel {
	namespace types {



		AARectangle::AARectangle(Vector2 size /*= Vector2()*/, Vector2 center /*= Vector2()*/, bool halfSize)
			: m_halfSize(size)
			, m_center(center)
		{
			// User passed full size
			// TODO: Make this the default and change examples
			if (!halfSize)
			{
				m_halfSize /= 2.0f;
			}
		}

		AARectangle::~AARectangle()
		{
		}

	}
}