#pragma once

#if defined(__linux__)

#include "core/IInitializer.h"
#include "renderer/LocalGL.h"
#include <X11/Xlib.h>

namespace perfectpixel::core {

class LinuxInitializer : public IInitializer
{
public:
    LinuxInitializer(Display *display);
    virtual ~LinuxInitializer();

public:
    virtual renderer::IWindow *createWindow() override;
    virtual void handleOsStep() override;
    virtual void exit() override;

private:
    Display *m_display;
};

} // namespace perfectpixel::core

#endif
