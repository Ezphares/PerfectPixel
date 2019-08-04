#pragma once

#include <graphics/WindowSettings.h>
#include <Resources/Image.h>

#include <Bedrock/InputCallback.h>

namespace perfectpixel {
namespace graphics {

class IWindow
{
public:
	typedef std::function<void(bool)> FocusCallback;
	typedef std::function<void(IWindow&, unsigned, unsigned)> SizeCallback;

	IWindow() noexcept {}
	virtual ~IWindow() {}

public:
	virtual void initialize(const WindowSettings &settings) = 0;
	virtual void activate() = 0;
	virtual void destroy() = 0;
	virtual WindowDimensions getDimensions() = 0;
	virtual bool isClosed() = 0;
	virtual void draw() = 0;
	virtual void setSplash(const resources::Image &image) = 0;

	virtual void setKeyCallback(bedrock::KeyCallback callback) = 0;
	virtual void setFocusCallback(FocusCallback callback) = 0;
	virtual void setResizeCallback(SizeCallback callback) = 0;
};

}
}
