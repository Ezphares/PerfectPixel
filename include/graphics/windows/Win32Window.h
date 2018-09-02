#pragma once

#ifdef _WIN32

#include <map>

#include <graphics/LocalGL.h>
#include <graphics/IWindow.h>

namespace perfectpixel {
namespace graphics {

class Win32Window : public IWindow
{
public:
	Win32Window(
		HINSTANCE hInstance,
		std::string wndClassName);
	virtual ~Win32Window();

public:
	virtual void initialize(const WindowSettings &settings);
	virtual void activate();
	virtual void draw();
	virtual void destroy();
	virtual WindowDimensions getDimensions();
	virtual bool isClosed();
	virtual void setSplash(const PNG &png);


private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	static std::map<HWND, Win32Window*> m_handleLookup;

	HBITMAP m_splash;
	HBRUSH m_brush;

	WNDCLASS m_wc;
	HWND m_hwnd;
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	WindowDimensions m_dimensions;

	HINSTANCE m_hInstance;
	std::string m_wndClassName;

	bool m_closed;
};

}
}

#endif
