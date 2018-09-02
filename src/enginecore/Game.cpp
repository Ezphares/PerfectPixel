#include <enginecore/Game.h>

#include <graphics/IWindow.h>

#include <boost/bind.hpp>
#include <chrono>
#include <thread>

namespace perfectpixel {
namespace core {

	Game::Game()
		: m_shouldExit(false)
		, m_entityManager()
		, m_physicsManager(&m_entityManager)
		, m_graphicsManager(&m_entityManager, boost::bind(&physics::PhysicsManager::getPosition, &m_physicsManager, _1))
		, m_targetUps(100)
		, m_splashFilename("splash.png")
{

}

void Game::run()
{
	preInit();

	showSplashScreen();

	graphics::WindowSettings mainWindowSettings;
	mainWindowSettings.title = "Untitled";
	mainWindowSettings.dimensions = graphics::WindowDimensions(640, 480);
	mainWindowSettings.type = graphics::WindowSettings::WINDOWED;

	// Virtual call, change window settings
	setupMainWindow(mainWindowSettings);

	// Create the window
	graphics::IWindow *mainWindow = createWindow(mainWindowSettings);
	mainWindow->activate();

	m_graphicsManager.initialize();

	gameStart();

	// Set up frame timers
	typedef std::chrono::steady_clock Clock;
	typedef std::chrono::duration<double> Duration;

	double deltaT = 1 / m_targetUps;
	double accumulator = 0;
	Clock::time_point lastTime = Clock::now();

	while (!m_shouldExit)
	{
		Clock::time_point currentTime = Clock::now();
		Duration frametime = currentTime - lastTime;
		lastTime = currentTime;

		accumulator += frametime.count();

		// TODO: Make sure we always run a step before first render? Or solve potential setup issues another way
		int safety = 10;
		while (accumulator >= deltaT && --safety)
		{
			update(deltaT);
			step();
			accumulator -= deltaT;
		}
		
		// Render step
		m_graphicsManager.drawAll(frametime.count());
		mainWindow->draw();
		m_initializer->handleOsStep();

		m_shouldExit = mainWindow->isClosed();
	}

	m_initializer->exit();
}

void Game::splashScreenUpdate(bool &closeSplash)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	closeSplash = true;
}

void Game::setup(IInitializer *initializer)
{
	m_instance->m_initializer = initializer;

	m_instance->run();
}

graphics::IWindow *Game::createWindow(const graphics::WindowSettings &settings)
{
	graphics::IWindow *window = m_initializer->createWindow();
	
	window->initialize(settings);

	return window;
}

void Game::exit()
{
	m_shouldExit = true;
}

void Game::update(double dt)
{

}

void Game::showSplashScreen()
{
	graphics::WindowSettings splashSettings;

	graphics::PNG splashPng = graphics::PNG::fromFile(m_splashFilename, true, false);
	splashSettings.title = "Splash";
	splashSettings.dimensions = graphics::WindowDimensions(splashPng.m_w, splashPng.m_h);
	splashSettings.type = graphics::WindowSettings::BORDERLESS;

	graphics::IWindow *splash = m_initializer->createWindow();
	splash->setSplash(splashPng);
	splash->initialize(splashSettings);
	splash->activate();

	bool shouldClose = false;
	while (!shouldClose)
	{
		splashScreenUpdate(shouldClose);
		splash->draw();
		m_initializer->handleOsStep();
	}

	splash->destroy();
}

}
}
