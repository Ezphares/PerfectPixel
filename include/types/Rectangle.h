#pragma once

#include <types/vectors.h>

namespace perfectpixel {
	namespace types {

	class Rectangle
	{
	public:
		Rectangle(Vector2 size = Vector2(), Vector2 center = Vector2());
		~Rectangle();

	public:
		Vector2 m_size, m_center;
	};

}
}
