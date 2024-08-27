#include "core/Game.h"

#ifdef _WIN32

#include <Windows.h>
#include "core/Win32Initializer.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nCmdShow;

	using namespace perfectpixel::core;

	Game::setup(new Win32Initializer(hInstance));

	return 0;
}

#endif
