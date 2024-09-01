#pragma once

#if defined(__linux__)

#include "renderer/IWindow.h"
#include <X11/Xlib.h>

namespace perfectpixel::renderer {

class X11Window : public IWindow
{
public:
    X11Window(Display *display);

    bool initialize(const WindowSettings &settings) override;
    void activate() override;
    void destroy() override;
    WindowDimensions getDimensions() override;
    bool isClosed() override;
    void startFrame() override;
    void draw() override;
    void setSplash(int w, int h, int c, unsigned char *buffer) override;
    void initImGui() override;
    void shutdownImGui() override;
    void setKeyCallback(bedrock::KeyCallback callback) override;
    void setFocusCallback(FocusCallback callback) override;
    void setResizeCallback(SizeCallback callback) override;

private:
    Display *m_display;
    Window m_window;
    GC m_gc;

    WindowDimensions m_dimensions;

    XImage *m_splashImage;
    Pixmap m_splashPixmap;
    GC m_splashGC;
};

} // namespace perfectpixel::renderer
#endif
