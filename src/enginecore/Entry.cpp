#include <enginecore/Game.h>

#ifdef _WIN32

#include <Windows.h>
#include <enginecore/Win32Initializer.h>

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	using namespace perfectpixel::core;

	Game::setup(new Win32Initializer(hInstance));

	return 0;
}

#endif
