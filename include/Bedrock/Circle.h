#pragma once

#include <Bedrock/vectors.h>

namespace perfectpixel {
	namespace bedrock {

		class Circle
		{
		public:
			Circle(float radius, Vector2 center = Vector2());
			~Circle();

		public:
			float m_radius;
			Vector2 m_center;
		};

	}
}