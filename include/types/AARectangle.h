#pragma once

#include <types/vectors.h>

namespace perfectpixel {
	namespace types {

	class AARectangle
	{
	public:
		AARectangle(Vector2 size = Vector2(), Vector2 center = Vector2());
		~AARectangle();

	public:
		Vector2 m_size, m_center;
	};

}
}
