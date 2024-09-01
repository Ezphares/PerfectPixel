#if defined(__linux__)

#include "renderer/windows/X11Window.h"

#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <iostream>

namespace perfectpixel::renderer {

X11Window::X11Window(Display *display)
    : m_display(display)
    , m_window()
    , m_gc(0)
    , m_splashImage(nullptr)
{}

bool X11Window::initialize(const WindowSettings &settings)
{
    m_dimensions = settings.dimensions;

    XSetWindowAttributes attributes{};

    m_window = XCreateWindow(
        m_display,
        DefaultRootWindow(m_display),
        0,
        0,
        settings.dimensions.first,
        settings.dimensions.second,
        24,
        CopyFromParent,
        InputOutput,
        DefaultVisual(m_display, XDefaultScreen(m_display)),
        0,
        &attributes);

    // Borderless
    if (settings.type != WindowSettings::WINDOWED)
    {
        struct Hints
        {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long inputMode;
            unsigned long status;
        } hints;
        Atom windowProperty;
        hints.flags       = 2;
        hints.decorations = 0;
        windowProperty    = XInternAtom(m_display, "_MOTIF_WM_HINTS", true);
        XChangeProperty(
            m_display,
            m_window,
            windowProperty,
            windowProperty,
            32,
            PropModeReplace,
            (unsigned char *)&hints,
            5);
    }

    XGCValues gcValues{};
    gcValues.foreground = BlackPixel(m_display, XDefaultScreen(m_display));
    gcValues.background = WhitePixel(m_display, XDefaultScreen(m_display));

    m_gc = XCreateGC(m_display, m_window, 0, &gcValues);

    XSelectInput(m_display, m_window, ExposureMask | KeyPressMask);
    XMapRaised(m_display, m_window);

    return false;
}

void X11Window::activate()
{
    if (m_splashImage)
    {
        XPutImage(
            m_display,
            m_splashPixmap,
            m_gc,
            m_splashImage,
            0,
            0,
            0,
            0,
            m_dimensions.first,
            m_dimensions.second);

        XEvent e;
        while (1)
        {
            XNextEvent(m_display, &e);

            if (e.type == Expose)
            {
                XCopyArea(
                    m_display,
                    m_splashPixmap,
                    m_window,
                    m_gc,
                    0,
                    0,
                    m_dimensions.first,
                    m_dimensions.second,
                    0,
                    0);
                XFlush(m_display);
                break;
            }
        }
    }
}

void X11Window::destroy()
{}

perfectpixel::renderer::WindowDimensions X11Window::getDimensions()
{
    throw std::logic_error("The method or operation is not implemented.");
}

bool X11Window::isClosed()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::startFrame()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::draw()
{}

void X11Window::setSplash(int w, int h, int c, unsigned char *buffer)
{
    int bufLen      = w * h * 4;
    char *altBuffer = (char *)malloc(bufLen);

    (void)c;

    if (c < 4)
    {
        memset(altBuffer, 0, bufLen);

        for (int i = 0; i < bufLen / 4; ++i)
        {
            memcpy(altBuffer + i * 4, buffer + i * c, c);
        }
    }
    else
    {
        memcpy(altBuffer, buffer, bufLen);
    }

    m_splashImage = XCreateImage(
        m_display, CopyFromParent, 24, ZPixmap, 0, altBuffer, w, h, 32, 0);
    m_splashPixmap
        = XCreatePixmap(m_display, XDefaultRootWindow(m_display), w, h, 24);
}

void X11Window::initImGui()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::shutdownImGui()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::setKeyCallback(bedrock::KeyCallback callback)
{
    (void)callback;
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::setFocusCallback(FocusCallback callback)
{
    (void)callback;
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::setResizeCallback(SizeCallback callback)
{
    (void)callback;
    throw std::logic_error("The method or operation is not implemented.");
}

} // namespace perfectpixel::renderer

#endif