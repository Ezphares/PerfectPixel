#if defined(__linux__)

#include "core/LinuxInitializer.h"

#include "renderer/windows/X11Window.h"

namespace perfectpixel::core {

LinuxInitializer::LinuxInitializer(Display *display)
    : m_display(display)
{}

LinuxInitializer::~LinuxInitializer()
{}

renderer::IWindow *LinuxInitializer::createWindow()
{
    return new renderer::X11Window();
}

void LinuxInitializer::handleOsStep()
{}

void LinuxInitializer::exit()
{
    XCloseDisplay(m_display);
}

} // namespace perfectpixel::core
#endif