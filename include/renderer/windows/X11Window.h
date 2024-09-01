#pragma once

#if defined(__linux__)

#include "renderer/IWindow.h"
#include <X11/Xlib.h>

namespace perfectpixel::renderer {

class X11Window : public IWindow
{
public:
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
};

} // namespace perfectpixel::renderer
#endif
