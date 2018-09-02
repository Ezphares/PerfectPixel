#ifdef _WIN32

#include <enginecore/Win32Initializer.h>

#include <graphics/windows/Win32Window.h>

#include <sstream>

namespace perfectpixel {
namespace core {

Win32Initializer::Win32Initializer(
	HINSTANCE hInstance)
	: m_classIndex(0)
	, m_hInstance(hInstance)
{
}

Win32Initializer::~Win32Initializer()
{
}

graphics::IWindow *Win32Initializer::createWindow()
{
	std::stringstream wndClass;
	wndClass << "wndclass-" << m_classIndex++;

	return new graphics::Win32Window(m_hInstance, wndClass.str().c_str());
}

void Win32Initializer::handleOsStep()
{
	MSG msg = { 0 };
	while (true)
	{
		BOOL messagePresent = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (messagePresent == 0)
		{
			break;
		}
		DispatchMessage(&msg);
	}
}

void Win32Initializer::exit()
{
	PostQuitMessage(0);
}

}
}

#endif
