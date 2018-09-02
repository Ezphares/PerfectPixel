#pragma once

#ifdef _WIN32

#include <graphics/LocalGL.h>
#include <enginecore/IInitializer.h>

namespace perfectpixel {
namespace core {

class Win32Initializer : public IInitializer
{
public:
	Win32Initializer(HINSTANCE hInstance);
	virtual ~Win32Initializer();

public:
	virtual graphics::IWindow *createWindow();
	virtual void handleOsStep();
	virtual void exit();

private:
	HINSTANCE m_hInstance;
	unsigned m_classIndex;
};

}
}

#endif
