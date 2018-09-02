#pragma once

namespace perfectpixel {

namespace graphics {
	class IWindow;
	struct WindowSettings;
}

	
namespace core {

class IInitializer
{
protected:
	IInitializer() {}
	IInitializer(const IInitializer&) = delete;

public:
	virtual ~IInitializer() {}

public:
	virtual graphics::IWindow *createWindow() = 0;
	virtual void handleOsStep() = 0;
	virtual void exit() = 0;

};

}
}
