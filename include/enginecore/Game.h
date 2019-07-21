#pragma once

#include <enginecore/IInitializer.h>

#include <EntityComponentSystem/EntityManager.h>
#include <EntityComponentSystem/ProcessorQueue.h>
#include <graphics/GraphicsManager.h>
#include <input/InputManager.h>

#include <Bedrock/numbers.h>

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
	virtual void setupCustomProcessors(ecs::ProcessorQueue &queue) {};

protected:
	graphics::IWindow *createWindow(const graphics::WindowSettings &settings);

	void exit();

private:
	void setupProcessors();
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
	ecs::ProcessorQueue m_processorQueue;
	input::InputManager m_inputManager;
	graphics::GraphicsManager m_graphicsManager;

private:
	// Use double over float, precision is required here
	double m_targetUps;

};

}
}

#define PP_GAME(cls) perfectpixel::core::Game *perfectpixel::core::Game::m_instance = new cls ();
