#pragma once

#include "core/IInitializer.h"

#include "core/FileResourceLocator.h"
#include "core/InputManager.h"
#include "core/ResourceManager.h"
#include "ecs/EntityManager.h"
#include "ecs/SystemQueue.h"
#include "renderer/RendererInterface.h"

#include "bedrock/numbers.h"

#include <string>

namespace perfectpixel::core {

class Game
{
public:
    Game();
    virtual ~Game(){};

public:
    static void setup(IInitializer *initializer);

    void run();

    void variableUpdate(double variableDeltaT);

    static void focusWrapper(void *instance, bool hasFocus);
    void focus(bool hasFocus);

    static void windowResizedWrapper(
        void *instance,
        renderer::IWindow &window,
        unsigned width,
        unsigned height);
    void
    windowResized(renderer::IWindow &window, unsigned width, unsigned height);

    ecs::Entity spawnTemplate(const std::string &data);

    // virtual bool prepareSplashScreen();

    virtual void preInit(){};
    virtual void setupMainWindow(renderer::WindowSettings &settings) = 0;
    virtual void registerResouces()
    {}
    virtual void gameStart() = 0;
    virtual void step(){};
    virtual void splashScreenUpdate(bool &closeSplash);
    virtual void setupCustomProcessors(ecs::SystemQueue &queue)
    {
        (void)queue;
    };

protected:
    renderer::IWindow *createWindow(const renderer::WindowSettings &settings);

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
    renderer::IWindow *m_mainWindow;

    bool m_shouldExit;

protected:
    ecs::SystemQueue m_processorQueue;
    input::InputManager m_inputManager;
    renderer::RendererInterface m_graphicsManager;
    core::FileResourceLocator m_fileResourceLocator;

private:
    // Use double over float, precision is required here
    double m_targetUps;
};

} // namespace perfectpixel::core

#define PP_GAME(cls)                                                           \
    perfectpixel::core::Game *perfectpixel::core::Game::m_instance = new cls();
