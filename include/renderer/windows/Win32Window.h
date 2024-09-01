#pragma once

#ifdef _WIN32

#include "renderer/IWindow.h"
#include "renderer/LocalGL.h"

#include "bedrock/InputEnums.h"

struct ImGuiViewport;

namespace perfectpixel { namespace renderer {

class Win32Window : public IWindow
{
public:
    Win32Window(HINSTANCE hInstance, std::string wndClassName);
    virtual ~Win32Window();

public:
    virtual bool initialize(const WindowSettings &settings) override;
    virtual void activate() override;
    virtual void startFrame() override;
    virtual void draw() override;
    virtual void destroy() override;
    virtual WindowDimensions getDimensions() override;
    virtual bool isClosed() override;
    virtual void setSplash(int w, int h, int c, unsigned char *buffer) override;
    virtual void setKeyCallback(bedrock::KeyCallback callback) override;
    virtual void setFocusCallback(FocusCallback callback) override;
    virtual void setResizeCallback(SizeCallback callback) override;

    virtual void initImGui() override;
    virtual void shutdownImGui() override;

private:
    bool wndProcImpl(
        LRESULT &retval, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK
    WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    static int pixelFormat(HDC deviceContext);
    struct WGL_WindowData
    {
        HDC hDC;
    };
    static bool createImGuiDevice(HWND hWnd, WGL_WindowData *data);
    static void cleanupImGuiDevice(HWND hWnd, WGL_WindowData *data);
    static void imGuiHook_CreateWindow(ImGuiViewport *viewport);
    static void imGuiHook_DestroyWindow(ImGuiViewport *viewport);
    static void imGuiHook_RenderWindow(ImGuiViewport *viewport, void *);
    static void imGuiHook_SwapBuffers(ImGuiViewport *viewport, void *);

    static bedrock::KeyboardButton mapVirtualKey(WPARAM virtualKey);

private:
    static PIXELFORMATDESCRIPTOR pfd;

    HBITMAP m_splash;
    HBRUSH m_brush;

    WNDCLASSEX m_wc;
    HWND m_hwnd;
    HDC m_deviceContext;
    HGLRC m_renderingContext;
    WindowDimensions m_dimensions;

    HINSTANCE m_hInstance;
    std::string m_wndClassName;

    bedrock::KeyCallback m_keyCallback;
    FocusCallback m_focusCallback;
    SizeCallback m_sizeCallback;

    bool m_closed;
    bool m_imgui;
};

}} // namespace perfectpixel::renderer

#endif
