#if defined(__linux__)

#include "renderer/windows/X11Window.h"

namespace perfectpixel::renderer {

bool X11Window::initialize(const WindowSettings &settings)
{
    (void)settings;
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::activate()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::destroy()
{
    throw std::logic_error("The method or operation is not implemented.");
}

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
{
    throw std::logic_error("The method or operation is not implemented.");
}

void X11Window::setSplash(int w, int h, int c, unsigned char *buffer)
{
    (void)w;
    (void)h;
    (void)c;
    (void)buffer;
    throw std::logic_error("The method or operation is not implemented.");
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