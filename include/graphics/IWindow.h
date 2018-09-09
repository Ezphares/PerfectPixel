#pragma once

#include <graphics/WindowSettings.h>
#include <graphics/PNG.h>

#include <types/InputCallback.h>

namespace perfectpixel {
namespace graphics {

class IWindow
{
public:
	typedef boost::function<void(bool)> FocusCallback;

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

	virtual void setKeyCallback(types::KeyCallback callback) = 0;
	virtual void setFocusCallback(FocusCallback callback) = 0;
};

}
}
