#ifdef _WIN32

#include <graphics/windows/Win32Window.h>


namespace perfectpixel {
namespace graphics {

std::map<HWND, Win32Window*> Win32Window::m_handleLookup = std::map<HWND, Win32Window*>();

Win32Window::Win32Window(
	HINSTANCE hInstance,
	std::string wndClassName)
	: m_splash(0)
	, m_brush((HBRUSH)(COLOR_BACKGROUND))
	, m_wc()
	, m_hwnd()
	, m_deviceContext()
	, m_renderingContext()
	, m_dimensions()
	, m_hInstance(hInstance)
	, m_wndClassName(wndClassName)
	, m_closed(false)
{

}

Win32Window::~Win32Window()
{
	if (m_brush != (HBRUSH)(COLOR_BACKGROUND))
	{
		DeleteObject(m_brush);
	}
}

void Win32Window::initialize(const WindowSettings &trySettings)
{
	WindowSettings settings = trySettings;

	m_dimensions = settings.dimensions;

	m_wc.lpfnWndProc = Win32Window::WndProc;
	m_wc.hInstance = m_hInstance;
	m_wc.hbrBackground = m_brush;
	m_wc.lpszClassName = m_wndClassName.c_str();
	m_wc.style = CS_OWNDC;

	if (!RegisterClass(&m_wc))
	{
		throw "Could not register window class";
	}

	if (settings.type == WindowSettings::FULLSCREEN)
	{
		DEVMODE dm;
		memset(&dm, 0, sizeof(DEVMODE));
		dm.dmSize = sizeof(DEVMODE);
		dm.dmPelsWidth = settings.dimensions.first;
		dm.dmPelsHeight = settings.dimensions.second;
		dm.dmBitsPerPel = 32;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Failed", "Failed", 0);
			settings.type = WindowSettings::WINDOWED;
		}
	}


	DWORD dwStyle{};
	DWORD dwExStyle{};

	switch (settings.type)
	{
	case WindowSettings::WINDOWED:
		dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		break;
	case WindowSettings::BORDERLESS:
		dwStyle = WS_POPUP | WS_VISIBLE;
		dwExStyle = WS_EX_APPWINDOW;
		break;
	case WindowSettings::FULLSCREEN:
		dwStyle = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		dwExStyle = WS_EX_APPWINDOW;
		break;
	}

	RECT windowRect;
	windowRect.left = 0;
	windowRect.top = 0;
	windowRect.right = settings.dimensions.first;
	windowRect.bottom = settings.dimensions.second;
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

	LONG
		width = windowRect.right - windowRect.left,
		height = windowRect.bottom - windowRect.top;

	int
		x = 0,
		y = 0;

	// Center non-fullscreen windows
	if (settings.type != WindowSettings::FULLSCREEN)
	{
		RECT desktopSize;
		HWND desktop = GetDesktopWindow();
		GetWindowRect(desktop, &desktopSize);
		x = (desktopSize.right - width) / 2;
		y = (desktopSize.bottom - height) / 2;
	}

	m_hwnd = CreateWindowEx(
		dwExStyle,
		m_wc.lpszClassName,
		settings.title.c_str(),
		dwStyle,
		x, y,
		width, height,
		0, 0, m_hInstance, 0);

	if (!m_hwnd)
	{
		throw "Could not create window";
	}
	
	m_handleLookup[m_hwnd] = this;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	m_deviceContext = GetDC(m_hwnd);
	SetPixelFormat(m_deviceContext, ChoosePixelFormat(m_deviceContext, &pfd), &pfd);

	m_renderingContext = wglCreateContext(m_deviceContext);
}

void Win32Window::activate()
{
	wglMakeCurrent(m_deviceContext, m_renderingContext);
}

void Win32Window::draw()
{
	SwapBuffers(m_deviceContext);
}

void Win32Window::destroy()
{
	wglDeleteContext(m_renderingContext);
	DestroyWindow(m_hwnd);
}

WindowDimensions Win32Window::getDimensions()
{
	return m_dimensions;
}

bool Win32Window::isClosed()
{
	return m_closed;
}

void Win32Window::setSplash(const PNG &png)
{
	unsigned char *buffer = png.m_buffer.m_idata;

	if (png.m_channels < 4)
	{
		buffer = new unsigned char[png.m_buffer.m_size / png.m_channels * 4];
		memset(buffer, 0, png.m_buffer.m_size / png.m_channels * 4);

		for (unsigned i = 0; i < png.m_buffer.m_size / png.m_channels; ++i)
		{
			memcpy(buffer + i * 4, png.m_buffer.m_idata + i * png.m_channels, png.m_channels);
		}
	}
	
	m_splash = CreateBitmap(png.m_w, png.m_h, 1, 32, buffer);
	m_brush = CreatePatternBrush(m_splash);
}

LRESULT CALLBACK Win32Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		m_handleLookup[hwnd]->m_closed = true;
		return DefWindowProc(hwnd, message, wParam, lParam);

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

}
}

#endif
