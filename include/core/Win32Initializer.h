#pragma once

#ifdef _WIN32

#include "core/IInitializer.h"
#include "renderer/LocalGL.h"

namespace perfectpixel::core {

class Win32Initializer : public IInitializer
{
public:
    Win32Initializer(HINSTANCE hInstance);
    virtual ~Win32Initializer();

public:
    virtual renderer::IWindow *createWindow();
    virtual void handleOsStep();
    virtual void exit();

private:
    HINSTANCE m_hInstance;
    unsigned m_classIndex;
};

} // namespace perfectpixel::core

#endif
