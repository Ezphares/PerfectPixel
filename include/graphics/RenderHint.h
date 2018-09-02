#pragma once

namespace perfectpixel {
	namespace graphics {

		enum RenderHints {
			RH_NONE,
			RH_STATIC = 1 << 0,
			RH_INSTANCE = 1 << 1,
			RH_SOFTALPHA = 1 << 2
		};
} }