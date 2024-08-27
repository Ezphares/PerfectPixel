#pragma once

namespace perfectpixel {

namespace renderer {
class IWindow;
struct WindowSettings;
} // namespace renderer

namespace core {

class IInitializer
{
protected:
    IInitializer()
    {}
    IInitializer(const IInitializer &) = delete;

public:
    virtual ~IInitializer()
    {}

public:
    virtual renderer::IWindow *createWindow() = 0;
    virtual void handleOsStep()               = 0;
    virtual void exit()                       = 0;
};

} // namespace core
} // namespace perfectpixel
