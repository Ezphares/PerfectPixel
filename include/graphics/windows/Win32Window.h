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
	virtual void setKeyCallback(types::KeyCallback callback);
	virtual void setFocusCallback(FocusCallback callback);
	virtual void setResizeCallback(SizeCallback callback);


private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	
private:
	static std::map<HWND, Win32Window*> m_handleLookup;
	static Win32Window* m_currentSetup;

	HBITMAP m_splash;
	HBRUSH m_brush;

	WNDCLASSEX m_wc;
	HWND m_hwnd;
	HDC m_deviceContext;
	HGLRC m_renderingContext;
	WindowDimensions m_dimensions;

	HINSTANCE m_hInstance;
	std::string m_wndClassName;

	types::KeyCallback m_keyCallback;
	FocusCallback m_focusCallback;
	SizeCallback m_sizeCallback;

	bool m_closed;
};

}
}

#endif
