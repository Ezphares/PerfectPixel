#pragma once
#include <types/vectors.h>

namespace perfectpixel {
	namespace types {

		class Line
		{
		public:
			Line(Vector2 direction, Vector2 referencePoint, PpFloat halfLength = Infinity);
			~Line();

		public:
			Vector2 m_directionVector;
			Vector2 m_referencePoint;
			PpFloat m_halfLength;
		};

	}
}



