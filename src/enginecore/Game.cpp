#include <enginecore/Game.h>

#include <EntityComponentSystem/LifecycleComponents.h>
#include <EntityComponentSystem/DebugProcessor.h>

#include <Physics/IntegratorProcessor.h>
#include <Physics/CollisionProcessor.h>

#include <graphics/IWindow.h>

#include <functional>

#include <chrono>
#include <thread>

namespace perfectpixel {
namespace core {

	Game::Game()
		: m_shouldExit(false)
		, m_inputManager()
		, m_graphicsManager([](ecs::Entity entity) {return ecs::TransformComponent::Position(entity); })
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

	PP_LOG(LEVEL_INFO, "Setting up window");

	// Virtual call, change window settings
	setupMainWindow(mainWindowSettings);

	// Create the window
	graphics::IWindow *mainWindow = createWindow(mainWindowSettings);
	mainWindow->activate();
	mainWindow->setKeyCallback(m_inputManager.getKeyCallback());
	mainWindow->setFocusCallback(std::bind(&Game::focus, this, std::placeholders::_1));
	mainWindow->setResizeCallback(std::bind(&Game::windowResized, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

	PP_LOG(LEVEL_INFO, "Setting up renderer");

	m_graphicsManager.initialize();
	graphics::CameraSettings camera;
	camera.m_center = { 0, 0 };
	camera.m_size = { 160, 120 };
	camera.m_scaleMode = graphics::CameraSettings::SCALE_STRETCH;
	m_graphicsManager.setMainCamera(camera);

	// Fire a resize event to initialize size dependent variables
	windowResized(*mainWindow, mainWindowSettings.dimensions.first, mainWindowSettings.dimensions.second);

	PP_LOG(LEVEL_INFO, "Setting up ECS processors");

	setupProcessors();

	PP_LOG(LEVEL_INFO, "Starting game");

	gameStart();

	// Set up frame timers
	typedef std::chrono::steady_clock Clock;
	typedef std::chrono::duration<double> Duration;

	double deltaT = 1.0 / m_targetUps;
	double accumulator = 0.0;
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
			cleanup();
			accumulator -= deltaT;
		}

		if (safety <= 0)
		{
			PP_LOG(LEVEL_WARN, "Throttling updates. This might happen when alt-tabbing, changing settings etc. "
				"If it happens during normal play, it is a sign of performance issues");
			PP_LOG(LEVEL_WARN, "  Last frame recorded as " << unsigned(frametime.count() * 1000) << " ms");
			accumulator = 0;
		}
		
		// Render step
		types::Logger::touchLog(types::Logger::LOG_TOUCH_RENDER);
		m_graphicsManager.drawAll(frametime.count());
		m_graphicsManager.cleanup();
		mainWindow->draw();

		// Handle input
		m_initializer->handleOsStep();
		m_inputManager.update();

		m_shouldExit = mainWindow->isClosed();
	}

	PP_LOG(LEVEL_INFO, "Shutting down...");

	m_initializer->exit();
}

void Game::focus(bool hasFocus)
{
	m_inputManager.clearState();
}

void Game::windowResized(graphics::IWindow &window, unsigned width, unsigned height)
{
	m_graphicsManager.setWindowSize({ static_cast<types::PpInt>(width), static_cast<types::PpInt>(height) });
}

void Game::splashScreenUpdate(bool &closeSplash)
{
//	std::this_thread::sleep_for(std::chrono::seconds(1));
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

void Game::setupProcessors()
{
	m_processorQueue.registerProcessor(new ecs::DebugProcessor(), 200, true);
	m_processorQueue.registerProcessor(new physics::IntegratorProcessor(), 240, true);
	m_processorQueue.registerProcessor(new physics::CollisionProcessor(), 250, true);

	setupCustomProcessors(m_processorQueue);
}

void Game::update(double dt)
{
	types::Logger::touchLog(types::Logger::LOG_TOUCH_UPDATE);

	types::PpFloat ppdt = static_cast<types::PpFloat>(dt);

	m_processorQueue.processAll(ppdt);
}

void Game::cleanup()
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
