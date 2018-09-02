#pragma once

#include <enginecore/IInitializer.h>

#include <worldgraph/EntityManager.h>
#include <graphics/GraphicsManager.h>
#include <physics/PhysicsManager.h>

#include <types/numbers.h>

#include <string>

namespace perfectpixel {
namespace core {

class Game
{
public:
	Game();
	virtual ~Game() {};

public:
	static void setup(IInitializer *initializer);

	void run();

	//virtual bool prepareSplashScreen();
	
	virtual void preInit() {};
	virtual void setupMainWindow(graphics::WindowSettings &settings) = 0;
	virtual void gameStart() = 0;
	virtual void step() {};
	virtual void splashScreenUpdate(bool &closeSplash);

protected:
	graphics::IWindow *createWindow(const graphics::WindowSettings &settings);

	void exit();

private:
	void update(double dt);
	void loadResources();
	void showSplashScreen();

protected:
	std::string m_splashFilename;

private:
	static Game *m_instance;

	IInitializer *m_initializer;

	bool m_shouldExit;

protected:
	world::EntityManager m_entityManager;
	physics::PhysicsManager m_physicsManager;
	graphics::GraphicsManager m_graphicsManager;

private:
	// Use double over PpFloat, precision is required here
	double m_targetUps;

};

}
}

#define PP_GAME(cls) perfectpixel::core::Game *perfectpixel::core::Game::m_instance = new cls ();
