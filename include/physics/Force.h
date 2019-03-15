#pragma once

#include <types/vectors.h>

namespace perfectpixel {
	namespace physics {

		const static types::PpInt FORCE_ID_INTERNAL_START = 1 << 16;
		const static types::PpInt FORCE_ID_GRAVITY = FORCE_ID_INTERNAL_START + 1;
		const static types::PpInt FORCE_ID_DRAG = FORCE_ID_INTERNAL_START + 2;

		struct Force {
			types::PpInt m_identifier;
			types::Vector3 m_vector;
			bool m_relativeDirection; // Relative direction is compared to FORWARD
			bool m_relativeMagnitude;

			/// Create a constant force representing gravity. Magnitude is in Units / Second^2
			inline static Force gravity(types::PpFloat magnitude = 1)
			{
				return Force{
					FORCE_ID_GRAVITY,
					types::Vector3::DOWN * magnitude,
					false, false};
			}

			/// Create a constant force representing drag. Magnitude is in Units / Second^2. Progressive drag gets stronger the faster an object moves
			inline static Force drag(types::PpFloat magnitude = 1, bool progressive = true)
			{
				return Force{
					FORCE_ID_DRAG,
					types::Vector3::FORWARD * magnitude,
					true, true };
			}
		};


	}
}
