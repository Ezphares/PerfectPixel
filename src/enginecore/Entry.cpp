
#ifdef _WIN32

#include <Windows.h>
#include <enginecore/Game.h>
#include <enginecore/Win32Initializer.h>

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	using namespace perfectpixel::core;

	Game::setup(new Win32Initializer(hInstance));

	return 0;
}

#endif
