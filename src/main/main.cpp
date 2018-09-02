#include <graphics/LocalGL.h>
#include <enginecore/Game.h>
#include <graphics/IWindow.h>
#include <graphics/Texture.h>
#include <types/vectors.h>

class MyTestGame : public perfectpixel::core::Game
{
	virtual void preInit()
	{

	}

	virtual void setupMainWindow(perfectpixel::graphics::WindowSettings &mainWindow)
	{
		mainWindow.dimensions = perfectpixel::graphics::WindowDimensions(1600, 900);
		mainWindow.title = "Engine Test";
	}

	virtual void gameStart()
	{
		perfectpixel::graphics::Texture *tex = new  perfectpixel::graphics::Texture(perfectpixel::graphics::PNG::fromFile("bullet-circ-12.png") );

		perfectpixel::graphics::Sprite *spr = new perfectpixel::graphics::Sprite(
			tex,
			{ 0,0 },
			{ 1, 1 });

		perfectpixel::graphics::SpriteComponent com1{ 1, spr,{ 0.5, 0.5 },{ 0, 0 }, 1 };
		perfectpixel::graphics::SpriteComponent com2{ 2, spr, {0.5, 0.5},{ -0.5, -0.5 }, 1 };

		m_graphicsManager.registerSprite(1, com1);
		m_graphicsManager.registerSprite(2, com2);

		MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
	}
};

PP_GAME(MyTestGame);
