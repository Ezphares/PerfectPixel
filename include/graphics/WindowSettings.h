#pragma once

#include <utility>
#include <string>

namespace perfectpixel {
namespace graphics {

typedef std::pair<unsigned, unsigned> WindowDimensions;

struct WindowSettings {
	enum WindowType {
		WINDOWED,
		BORDERLESS,
		FULLSCREEN
	};

	WindowDimensions dimensions;
	std::string title;
	WindowType type;
};

}
}
