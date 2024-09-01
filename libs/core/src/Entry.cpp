#include "core/Game.h"

#ifdef _WIN32

#include "core/Win32Initializer.h"
#include <Windows.h>

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nCmdShow;

    using namespace perfectpixel::core;

    Game::setup(new Win32Initializer(hInstance));

    return 0;
}

#endif

#if defined(__linux__)

#include "core/LinuxInitializer.h"
#include <X11/Xlib.h>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    using namespace perfectpixel::core;

    Game::setup(new LinuxInitializer(XOpenDisplay(NULL)));

    return 0;
}
#endif