#pragma once

#include <functional>

namespace perfectpixel {
	namespace bedrock {

		typedef uint32_t KeyCode;

		enum KeyEvent {
			PP_KEYUP,
			PP_KEYDOWN
		};

		typedef std::function<void(KeyCode, KeyEvent)> KeyCallback;

	}
}
