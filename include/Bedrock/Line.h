#pragma once
#include <Bedrock/vectors.h>

namespace perfectpixel {
	namespace bedrock {

		class Line
		{
		public:
			Line(Vector2 direction, Vector2 referencePoint, float halfLength = Infinity);
			~Line();

		public:
			Vector2 m_directionVector;
			Vector2 m_referencePoint;
			float m_halfLength;
		};

	}
}



