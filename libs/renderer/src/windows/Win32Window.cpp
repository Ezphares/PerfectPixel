#ifdef _WIN32

#include "renderer/windows/Win32Window.h"

#include "bedrock/PpException.h"

// clang-format off
#include "imgui.h"
#include "backends/imgui_impl_win32.h"
// clang-format on

extern IMGUI_IMPL_API
    LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

static HGLRC g_ImguiRC = NULL;

namespace perfectpixel { namespace renderer {

std::map<HWND, Win32Window *> Win32Window::m_handleLookup
    = std::map<HWND, Win32Window *>();
Win32Window *Win32Window::m_currentSetup = NULL;
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

void Win32Window::initialize(const WindowSettings &trySettings)
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
        throw bedrock::PpException("Could not register window class");
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

    m_currentSetup = this;
    m_hwnd         = CreateWindowEx(
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
        0);

    if (!m_hwnd)
    {
        throw bedrock::PpException("Could not create window");
    }

    m_handleLookup[m_hwnd] = this;
    m_currentSetup         = NULL;

    m_deviceContext = GetDC(m_hwnd);
    SetPixelFormat(m_deviceContext, pixelFormat(m_deviceContext), &pfd);

    m_renderingContext = wglCreateContext(m_deviceContext);
    wglMakeCurrent(m_deviceContext, m_renderingContext);
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

LRESULT CALLBACK
Win32Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    Win32Window *self = Win32Window::m_currentSetup;
    auto it           = m_handleLookup.find(hwnd);
    if (it != m_handleLookup.end())
    {
        self = it->second;
    }

    if (self->m_imgui)
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam))
        {
            return true;
        }
    }

    switch (message)
    {
    case WM_CLOSE:
        if (self)
        {
            self->m_closed = true;
        }
        break;

    case WM_KEYDOWN:
        if (self && self->m_keyCallback.m_func)
        {
            self->m_keyCallback.m_func(
                self->m_keyCallback.m_instance,
                static_cast<bedrock::KeyCode>(wParam),
                bedrock::PP_KEYDOWN);
            return 0;
        }
        break;

    case WM_KEYUP:
        if (self && self->m_keyCallback.m_func)
        {
            self->m_keyCallback.m_func(
                self->m_keyCallback.m_instance,
                static_cast<bedrock::KeyCode>(wParam),
                bedrock::PP_KEYUP);
            return 0;
        }
        break;

    case WM_KILLFOCUS:
        if (self && self->m_focusCallback.m_func)
        {
            self->m_focusCallback.m_func(
                self->m_focusCallback.m_instance, false);
        }
        break;

    case WM_SETFOCUS:
        if (self && self->m_focusCallback.m_func)
        {
            self->m_focusCallback.m_func(
                self->m_focusCallback.m_instance, true);
        }
        break;

    case WM_SIZE:
        if (self && self->m_sizeCallback.m_func)
        {
            unsigned width{static_cast<unsigned>(lParam & ((1 << 16) - 1))},
                height{static_cast<unsigned>(lParam >> 16)};

            self->m_sizeCallback.m_func(
                self->m_sizeCallback.m_instance, *self, width, height);
        }
        break;

    default:
        break;
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

}} // namespace perfectpixel::renderer

#endif
