#ifdef _WIN32

#include "bedrock/Assert.h"

#include "core/Win32Initializer.h"

#include "renderer/windows/Win32Window.h"

#include <sstream>

namespace perfectpixel::core {

Win32Initializer::Win32Initializer(HINSTANCE hInstance)
    : m_classIndex(0)
    , m_hInstance(hInstance)
{
    bedrock::assertHandler = &assertHandler;
}

Win32Initializer::~Win32Initializer()
{}

renderer::IWindow *Win32Initializer::createWindow()
{
    std::stringstream wndClass;
    wndClass << "wndclass-" << m_classIndex++;

    return new renderer::Win32Window(m_hInstance, wndClass.str().c_str());
}

void Win32Initializer::handleOsStep()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Win32Initializer::exit()
{
    PostQuitMessage(0);
}

void Win32Initializer::assertHandler(const char *message)
{
    int response = MessageBox(
        nullptr, message, "ASSERTION FAILED!", MB_ABORTRETRYIGNORE);
    switch (response)
    {
    case IDABORT:
        ::exit(1);
        break;
    case IDRETRY:
        __debugbreak();
        break;
    }
}

} // namespace perfectpixel::core

#endif
