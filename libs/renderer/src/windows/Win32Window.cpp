#ifdef _WIN32

#include "renderer/windows/Win32Window.h"

// clang-format off
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
// clang-format on

#include <unordered_map>

extern IMGUI_IMPL_API
    LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

static HGLRC g_ImguiRC = NULL;

namespace perfectpixel::renderer {

PIXELFORMATDESCRIPTOR Win32Window::pfd{0};

Win32Window::Win32Window(HINSTANCE hInstance, std::string wndClassName)
    : m_splash(0)
    , m_brush((HBRUSH)(COLOR_BACKGROUND))
    , m_wc()
    , m_hwnd()
    , m_deviceContext()
    , m_renderingContext()
    , m_dimensions()
    , m_hInstance(hInstance)
    , m_wndClassName(wndClassName)
    , m_keyCallback()
    , m_focusCallback()
    , m_sizeCallback()
    , m_closed(false)
    , m_imgui(false)
{}

Win32Window::~Win32Window()
{
    if (m_brush != (HBRUSH)(COLOR_BACKGROUND))
    {
        DeleteObject(m_brush);
    }
}

bool Win32Window::initialize(const WindowSettings &trySettings)
{
    WindowSettings settings = trySettings;

    m_dimensions = settings.dimensions;

    ZeroMemory(&m_wc, sizeof(WNDCLASSEX));
    m_wc.cbSize        = sizeof(WNDCLASSEX);
    m_wc.lpfnWndProc   = Win32Window::WndProc;
    m_wc.hInstance     = m_hInstance;
    m_wc.hbrBackground = m_brush;
    m_wc.lpszClassName = m_wndClassName.c_str();
    m_wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    m_wc.style         = CS_OWNDC /* | CS_HREDRAW | CS_VREDRAW */;

    if (!RegisterClassEx(&m_wc))
    {
        return false;
    }

    if (settings.type == WindowSettings::FULLSCREEN)
    {
        DEVMODE dm;
        memset(&dm, 0, sizeof(DEVMODE));
        dm.dmSize       = sizeof(DEVMODE);
        dm.dmPelsWidth  = settings.dimensions.first;
        dm.dmPelsHeight = settings.dimensions.second;
        dm.dmBitsPerPel = 32;
        dm.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN)
            != DISP_CHANGE_SUCCESSFUL)
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
        dwStyle   = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        break;
    case WindowSettings::BORDERLESS:
        dwStyle   = WS_POPUP | WS_VISIBLE;
        dwExStyle = WS_EX_APPWINDOW;
        break;
    case WindowSettings::FULLSCREEN:
        dwStyle   = WS_POPUP | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        dwExStyle = WS_EX_APPWINDOW;
        break;
    }

    RECT windowRect;
    windowRect.left   = 0;
    windowRect.top    = 0;
    windowRect.right  = settings.dimensions.first;
    windowRect.bottom = settings.dimensions.second;
    AdjustWindowRectEx(
        &windowRect,
        dwStyle,
        FALSE,
        dwExStyle); // Adjust Window To True Requested Size

    LONG width  = windowRect.right - windowRect.left,
         height = windowRect.bottom - windowRect.top;

    int x = 0, y = 0;

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
        x,
        y,
        width,
        height,
        0,
        0,
        m_hInstance,
        this);

    if (!m_hwnd)
    {
        return false;
    }

    m_deviceContext = GetDC(m_hwnd);
    SetPixelFormat(m_deviceContext, pixelFormat(m_deviceContext), &pfd);

    m_renderingContext = wglCreateContext(m_deviceContext);
    wglMakeCurrent(m_deviceContext, m_renderingContext);

    return true;
}

void Win32Window::activate()
{
    wglMakeCurrent(m_deviceContext, m_renderingContext);
}

void Win32Window::startFrame()
{
    if (m_imgui)
    {
        ImGui_ImplWin32_NewFrame();
    }
}

void Win32Window::draw()
{
    SwapBuffers(m_deviceContext);
}

void Win32Window::destroy()
{
    wglDeleteContext(m_renderingContext);
    DestroyWindow(m_hwnd);
    UnregisterClass(m_wndClassName.c_str(), m_hInstance);
}

WindowDimensions Win32Window::getDimensions()
{
    return m_dimensions;
}

bool Win32Window::isClosed()
{
    return m_closed;
}

void Win32Window::setSplash(int w, int h, int c, unsigned char *buffer)
{
    const int bufLen         = w * h * c;
    unsigned char *altBuffer = nullptr;

    if (c < 4)
    {
        altBuffer = new unsigned char[bufLen / c * 4];
        memset(altBuffer, 0, bufLen / c * 4);

        for (int i = 0; i < bufLen / c; ++i)
        {
            memcpy(altBuffer + i * 4, buffer + i * c, c);
        }
    }

    m_splash = CreateBitmap(w, h, 1, 32, altBuffer ? altBuffer : buffer);
    m_brush  = CreatePatternBrush(m_splash);

    if (altBuffer)
        delete[] altBuffer;
}

void Win32Window::setKeyCallback(bedrock::KeyCallback callback)
{
    m_keyCallback = callback;
}

void Win32Window::setFocusCallback(FocusCallback callback)
{
    m_focusCallback = callback;
}

void Win32Window::setResizeCallback(SizeCallback callback)
{
    m_sizeCallback = callback;
}

void Win32Window::initImGui()
{
    ImGui_ImplWin32_InitForOpenGL(m_hwnd);
    g_ImguiRC = m_renderingContext;

    ImGuiIO &io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiPlatformIO &platformIO       = ImGui::GetPlatformIO();
        platformIO.Renderer_CreateWindow  = &imGuiHook_CreateWindow;
        platformIO.Renderer_DestroyWindow = &imGuiHook_DestroyWindow;
        platformIO.Renderer_SwapBuffers   = &imGuiHook_SwapBuffers;
        platformIO.Platform_RenderWindow  = &imGuiHook_RenderWindow;
    }

    m_imgui = true;
}

void Win32Window::shutdownImGui()
{
    ImGui_ImplWin32_Shutdown();
}

bool Win32Window::wndProcImpl(
    LRESULT &retval, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    retval = 0;
    if (m_imgui
        && ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
    {
        retval = 1;
        return true;
    }

    switch (message)
    {
    case WM_CLOSE:
        m_closed = true;
        break;

    case WM_KEYDOWN:
        if (m_keyCallback.m_func)
        {
            m_keyCallback.m_func(
                m_keyCallback.m_instance,
                mapVirtualKey(wParam),
                bedrock::PP_KEYDOWN);
            return true;
        }
        break;

    case WM_KEYUP:
        if (m_keyCallback.m_func)
        {
            m_keyCallback.m_func(
                m_keyCallback.m_instance,
                mapVirtualKey(wParam),
                bedrock::PP_KEYUP);
            return true;
        }
        break;

    case WM_SETFOCUS:
    case WM_KILLFOCUS:
        if (m_focusCallback.m_func)
        {
            m_focusCallback.m_func(
                m_focusCallback.m_instance, message == WM_SETFOCUS);
        }
        break;

    case WM_SIZE:
        if (m_sizeCallback.m_func)
        {
            const unsigned width  = LOWORD(lParam);
            const unsigned height = HIWORD(lParam);

            m_sizeCallback.m_func(
                m_sizeCallback.m_instance, *this, width, height);
        }
        break;

    default:
        break;
    }

    return false;
}

LRESULT CALLBACK
Win32Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_CREATE)
    {
        CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT *>(lParam);
        SetWindowLongPtr(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
    }

    Win32Window *window = reinterpret_cast<Win32Window *>(
        GetWindowLongPtr(hwnd, GWLP_USERDATA));

    LRESULT retval = 0;
    if (window && window->wndProcImpl(retval, hwnd, message, wParam, lParam))
    {
        return retval;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

int Win32Window::pixelFormat(HDC deviceContext)
{
    if (pfd.nSize == 0)
    {
        pfd.nSize    = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags
            = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType   = PFD_TYPE_RGBA;
        pfd.cColorBits   = 32;
        pfd.cStencilBits = 8;
        pfd.cDepthBits   = 24;
    }

    return ::ChoosePixelFormat(deviceContext, &pfd);
}

bool Win32Window::createImGuiDevice(HWND hWnd, WGL_WindowData *data)
{
    HDC hDc      = ::GetDC(hWnd);
    const int pf = pixelFormat(hDc);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_ImguiRC)
        g_ImguiRC = wglCreateContext(data->hDC);

    wglMakeCurrent(data->hDC, g_ImguiRC);
    wglSwapIntervalEXT(0);

    return true;
}

void Win32Window::cleanupImGuiDevice(HWND hWnd, WGL_WindowData *data)
{
    wglMakeCurrent(nullptr, nullptr);
    ::ReleaseDC(hWnd, data->hDC);
}

void Win32Window::imGuiHook_CreateWindow(ImGuiViewport *viewport)
{
    assert(viewport->RendererUserData == NULL);

    WGL_WindowData *data = IM_NEW(WGL_WindowData);
    createImGuiDevice((HWND)viewport->PlatformHandle, data);
    viewport->RendererUserData = data;
}

void Win32Window::imGuiHook_DestroyWindow(ImGuiViewport *viewport)
{
    if (viewport->RendererUserData != NULL)
    {
        WGL_WindowData *data = (WGL_WindowData *)viewport->RendererUserData;
        cleanupImGuiDevice((HWND)viewport->PlatformHandle, data);
        IM_DELETE(data);
        viewport->RendererUserData = NULL;
    }
}

void Win32Window::imGuiHook_RenderWindow(ImGuiViewport *viewport, void *)
{
    // Activate the platform window DC in the OpenGL rendering context
    if (WGL_WindowData *data = (WGL_WindowData *)viewport->RendererUserData)
        wglMakeCurrent(data->hDC, g_ImguiRC);
}

void Win32Window::imGuiHook_SwapBuffers(ImGuiViewport *viewport, void *)
{
    if (WGL_WindowData *data = (WGL_WindowData *)viewport->RendererUserData)
        ::SwapBuffers(data->hDC);
}

bedrock::KeyboardButton Win32Window::mapVirtualKey(WPARAM virtualKey)
{
    const static std::unordered_map<WPARAM, bedrock::KeyboardButton> mapping{
        {VK_BACK, bedrock::KeyboardButton::Backspace},
        {VK_TAB, bedrock::KeyboardButton::Tab},
        {VK_RETURN, bedrock::KeyboardButton::Enter},
        {VK_PAUSE, bedrock::KeyboardButton::Pause},
        {VK_CAPITAL, bedrock::KeyboardButton::CapsLock},
        {VK_ESCAPE, bedrock::KeyboardButton::Escape},
        {VK_SPACE, bedrock::KeyboardButton::Space},
        {VK_PRIOR, bedrock::KeyboardButton::PageUp},
        {VK_NEXT, bedrock::KeyboardButton::PageDown},
        {VK_END, bedrock::KeyboardButton::End},
        {VK_HOME, bedrock::KeyboardButton::Home},
        {VK_LEFT, bedrock::KeyboardButton::Left},
        {VK_UP, bedrock::KeyboardButton::Up},
        {VK_RIGHT, bedrock::KeyboardButton::Right},
        {VK_DOWN, bedrock::KeyboardButton::Down},
        {VK_SNAPSHOT, bedrock::KeyboardButton::PrintScreen},
        {VK_INSERT, bedrock::KeyboardButton::Insert},
        {VK_DELETE, bedrock::KeyboardButton::Delete},
        {0x30, bedrock::KeyboardButton::Digit0},
        {0x31, bedrock::KeyboardButton::Digit1},
        {0x32, bedrock::KeyboardButton::Digit2},
        {0x33, bedrock::KeyboardButton::Digit3},
        {0x34, bedrock::KeyboardButton::Digit4},
        {0x35, bedrock::KeyboardButton::Digit5},
        {0x36, bedrock::KeyboardButton::Digit6},
        {0x37, bedrock::KeyboardButton::Digit7},
        {0x38, bedrock::KeyboardButton::Digit8},
        {0x39, bedrock::KeyboardButton::Digit9},
        {0x41, bedrock::KeyboardButton::LetterA},
        {0x42, bedrock::KeyboardButton::LetterB},
        {0x43, bedrock::KeyboardButton::LetterC},
        {0x44, bedrock::KeyboardButton::LetterD},
        {0x45, bedrock::KeyboardButton::LetterE},
        {0x46, bedrock::KeyboardButton::LetterF},
        {0x47, bedrock::KeyboardButton::LetterG},
        {0x48, bedrock::KeyboardButton::LetterH},
        {0x49, bedrock::KeyboardButton::LetterI},
        {0x4a, bedrock::KeyboardButton::LetterJ},
        {0x4b, bedrock::KeyboardButton::LetterK},
        {0x4c, bedrock::KeyboardButton::LetterL},
        {0x4d, bedrock::KeyboardButton::LetterM},
        {0x4e, bedrock::KeyboardButton::LetterN},
        {0x4f, bedrock::KeyboardButton::LetterO},
        {0x50, bedrock::KeyboardButton::LetterP},
        {0x51, bedrock::KeyboardButton::LetterQ},
        {0x52, bedrock::KeyboardButton::LetterR},
        {0x53, bedrock::KeyboardButton::LetterS},
        {0x54, bedrock::KeyboardButton::LetterT},
        {0x55, bedrock::KeyboardButton::LetterU},
        {0x56, bedrock::KeyboardButton::LetterV},
        {0x57, bedrock::KeyboardButton::LetterW},
        {0x58, bedrock::KeyboardButton::LetterX},
        {0x59, bedrock::KeyboardButton::LetterY},
        {0x5a, bedrock::KeyboardButton::LetterZ},
        {VK_NUMPAD0, bedrock::KeyboardButton::Numpad0},
        {VK_NUMPAD1, bedrock::KeyboardButton::Numpad1},
        {VK_NUMPAD2, bedrock::KeyboardButton::Numpad2},
        {VK_NUMPAD3, bedrock::KeyboardButton::Numpad3},
        {VK_NUMPAD4, bedrock::KeyboardButton::Numpad4},
        {VK_NUMPAD5, bedrock::KeyboardButton::Numpad5},
        {VK_NUMPAD6, bedrock::KeyboardButton::Numpad6},
        {VK_NUMPAD7, bedrock::KeyboardButton::Numpad7},
        {VK_NUMPAD8, bedrock::KeyboardButton::Numpad8},
        {VK_NUMPAD9, bedrock::KeyboardButton::Numpad9},
        {VK_MULTIPLY, bedrock::KeyboardButton::NumpadMul},
        {VK_ADD, bedrock::KeyboardButton::NumpadAdd},
        {VK_SEPARATOR, bedrock::KeyboardButton::NumpadComma},
        {VK_SUBTRACT, bedrock::KeyboardButton::NumpadSub},
        {VK_DIVIDE, bedrock::KeyboardButton::NumpadDiv},
        {VK_F1, bedrock::KeyboardButton::F1},
        {VK_F2, bedrock::KeyboardButton::F2},
        {VK_F3, bedrock::KeyboardButton::F3},
        {VK_F4, bedrock::KeyboardButton::F4},
        {VK_F5, bedrock::KeyboardButton::F5},
        {VK_F6, bedrock::KeyboardButton::F6},
        {VK_F7, bedrock::KeyboardButton::F7},
        {VK_F8, bedrock::KeyboardButton::F8},
        {VK_F9, bedrock::KeyboardButton::F9},
        {VK_F10, bedrock::KeyboardButton::F10},
        {VK_F11, bedrock::KeyboardButton::F11},
        {VK_F12, bedrock::KeyboardButton::F12},
        {VK_NUMLOCK, bedrock::KeyboardButton::Numlock},
        {VK_SCROLL, bedrock::KeyboardButton::ScrollLock},
        {VK_LSHIFT, bedrock::KeyboardButton::LeftShift},
        {VK_RSHIFT, bedrock::KeyboardButton::RightShift},
        {VK_RCONTROL, bedrock::KeyboardButton::RightControl},
        {VK_LCONTROL, bedrock::KeyboardButton::LeftControl},
        {VK_RMENU, bedrock::KeyboardButton::RightAlt},
        {VK_LMENU, bedrock::KeyboardButton::LeftAlt},
    };

    auto it = mapping.find(virtualKey);

    return (it != mapping.end()) ? it->second
                                 : bedrock::KeyboardButton::Unsupported;
}
} // namespace perfectpixel::renderer

#endif
