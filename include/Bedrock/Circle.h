#pragma once

#include <Bedrock/vectors.h>

namespace perfectpixel {
	namespace bedrock {

		class Circle
		{
		public:
			Circle(PpFloat radius, Vector2 center = Vector2());
			~Circle();

		public:
			PpFloat m_radius;
			Vector2 m_center;
		};

	}
}