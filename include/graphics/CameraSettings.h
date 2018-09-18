#pragma once

#include <types/vectors.h>

namespace perfectpixel {
	namespace graphics {

		struct CameraSettings {
			enum ScaleMode
			{
				SCALE_STRETCH,
				SCALE_BARS,
				SCALE_ENLARGE_X,
				SCALE_ENLARGE_Y
			};
			
			types::Vector2 m_center;
			types::Vector2 m_size;
			ScaleMode m_scaleMode;
		};
		
} 
}
