#pragma once

#include <types/vectors.h>

namespace perfectpixel {
	namespace types {

		class Circle
		{
		public:
			Circle(PpFloat diameter, Vector2 center = Vector2());
			~Circle();

		public:
			PpFloat m_diameter;
			Vector2 m_center;
		};

	}
}