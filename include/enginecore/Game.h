#pragma once

#include <enginecore/IInitializer.h>

#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/ComponentRegistry.h>
#include <graphics/GraphicsManager.h>
#include <physics/PhysicsManager.h>
#include <input/InputManager.h>
#include <behaviour/BehaviourManager.h>

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
	void focus(bool hasFocus);
	void windowResized(graphics::IWindow &window, unsigned width, unsigned height);

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
	void cleanup();
	void loadResources();
	void showSplashScreen();

protected:
	std::string m_splashFilename;

private:
	static Game *m_instance;

	IInitializer *m_initializer;

	bool m_shouldExit;

protected:
	api::ManagerInterface m_managerInterface;
	world::EntityManager m_entityManager;
	world::ComponentRegistry m_componentRegistry;
	physics::PhysicsManager m_physicsManager;
	input::InputManager m_inputManager;
	graphics::GraphicsManager m_graphicsManager;
	behaviour::BehaviourManager m_behaviourManager;

private:
	// Use double over PpFloat, precision is required here
	double m_targetUps;

};

}
}

#define PP_GAME(cls) perfectpixel::core::Game *perfectpixel::core::Game::m_instance = new cls ();
