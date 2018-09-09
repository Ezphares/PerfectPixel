#pragma once

#include <boost/function.hpp>

namespace perfectpixel {
	namespace types {

		typedef uint32_t KeyCode;

		enum KeyEvent {
			PP_KEYUP,
			PP_KEYDOWN
		};

		typedef boost::function<void(KeyCode, KeyEvent)> KeyCallback;

	}
}
