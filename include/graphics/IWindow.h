#pragma once

#include <graphics/WindowSettings.h>
#include <graphics/PNG.h>

namespace perfectpixel {
namespace graphics {

class IWindow
{
public:
	IWindow() {}
	virtual ~IWindow() {}

public:
	virtual void initialize(const WindowSettings &settings) = 0;
	virtual void activate() = 0;
	virtual void destroy() = 0;
	virtual WindowDimensions getDimensions() = 0;
	virtual bool isClosed() = 0;
	virtual void draw() = 0;
	virtual void setSplash(const PNG &png) = 0;
};

}
}
