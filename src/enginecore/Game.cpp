#include <enginecore/Game.h>

#include <Resources/PNGImage.h>
#include <Resources/Sprite.h>
#include <Resources/Template.h>

#include <EntityComponentSystem/DebugProcessor.h>
#include <EntityComponentSystem/LifeCycleSystem.h>
#include <EntityComponentSystem/LifecycleComponents.h>

#include <physics/CollisionSystem.h>
#include <physics/IntegratorSystem.h>

#include <graphics/IWindow.h>
#include <graphics/UISystem.h>

#include <serialization/YAMLSerializer.h>

#include <chrono>
#include <thread>

namespace perfectpixel { namespace core {

Game::Game()
    : m_splashFilename("splash.png")
    , m_shouldExit(false)
    , m_processorQueue()
    , m_inputManager()
    , m_graphicsManager()
    , m_fileResourceLocator()
    , m_targetUps(100.0)
{}

void Game::run()
{
    preInit();

    showSplashScreen();

    graphics::WindowSettings mainWindowSettings;
    mainWindowSettings.title      = "Untitled";
    mainWindowSettings.dimensions = graphics::WindowDimensions(640, 480);
    mainWindowSettings.type       = graphics::WindowSettings::WINDOWED;

    PP_LOG(LEVEL_INFO, "Setting up window");

    // Virtual call, change window settings
    setupMainWindow(mainWindowSettings);

    // Create the window
    graphics::IWindow *mainWindow = createWindow(mainWindowSettings);
    mainWindow->activate();
    mainWindow->setKeyCallback(m_inputManager.getKeyCallback());

    graphics::IWindow::FocusCallback focusCb;
    focusCb.m_func     = &Game::focusWrapper;
    focusCb.m_instance = this;
    mainWindow->setFocusCallback(focusCb);

    graphics::IWindow::SizeCallback sizeCb;
    sizeCb.m_func     = &Game::windowResizedWrapper;
    sizeCb.m_instance = this;
    mainWindow->setResizeCallback(sizeCb);

    PP_LOG(LEVEL_INFO, "Setting up renderer");

    m_graphicsManager.initialize();
    graphics::CameraSettings camera;
    camera.m_center    = {0, 0};
    camera.m_size      = {160, 120};
    camera.m_scaleMode = graphics::CameraSettings::SCALE_STRETCH;
    m_graphicsManager.setMainCamera(camera);

    // Fire a resize event to initialize size dependent variables
    windowResized(
        *mainWindow,
        mainWindowSettings.dimensions.first,
        mainWindowSettings.dimensions.second);

    PP_LOG(LEVEL_INFO, "Setting up ECS processors");

    setupProcessors();

    PP_LOG(LEVEL_INFO, "Starting game");

    gameStart();

    // Set up frame timers
    typedef std::chrono::steady_clock Clock;
    typedef std::chrono::duration<double> Duration;

    double deltaT              = 1.0 / m_targetUps;
    double accumulator         = 0.0;
    Clock::time_point lastTime = Clock::now();

    while (!m_shouldExit)
    {
        Clock::time_point currentTime = Clock::now();
        Duration frametime            = currentTime - lastTime;
        lastTime                      = currentTime;

        accumulator += frametime.count();

        // TODO: Make sure we always run a step before first render? Or solve
        // potential setup issues another way
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
            PP_LOG(
                LEVEL_WARN,
                "Throttling updates. This might happen when alt-tabbing, "
                "changing settings etc. "
                "If it happens during normal play, it is a sign of performance "
                "issues");
            PP_LOG(
                LEVEL_WARN,
                "  Last frame recorded as "
                    << unsigned(frametime.count() * 1000) << " ms");
            accumulator = 0;
        }

        // Render step
        bedrock::Logger::touchLog(bedrock::Logger::LOG_TOUCH_RENDER);
        m_processorQueue.renderAll(static_cast<float>(frametime.count()));
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

void Game::focusWrapper(void *instance, bool hasFocus)
{
    reinterpret_cast<Game *>(instance)->focus(hasFocus);
}

void Game::focus(bool hasFocus)
{
    (void)hasFocus;
    m_inputManager.clearState();
}

void Game::windowResizedWrapper(
    void *instance, graphics::IWindow &window, unsigned width, unsigned height)
{
    reinterpret_cast<Game *>(instance)->windowResized(window, width, height);
}

void Game::windowResized(
    graphics::IWindow &window, unsigned width, unsigned height)
{
    (void)window;
    m_graphicsManager.setWindowSize(
        {static_cast<int32_t>(width), static_cast<int32_t>(height)});
}

ecs::Entity Game::spawnTemplate(const std::string &data)
{
    serialization::YAMLSerializer yaml;
    yaml.loadBuffer(data);
    yaml.m_hash    = &bedrock::crc32;
    yaml.m_reverse = &ecs::ReflectionTable::Reverse;

    ecs::Entity entity = ecs::EntityManager::getInstance()->create();
    ecs::ReflectionTable::getInstance()->deserialize(yaml, entity);

    return entity;
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
    m_processorQueue.registerSystem(new ecs::LifeCycleSystem(), 0, true);
    m_processorQueue.registerSystem(new ecs::DebugProcessor(), 200, true);
    m_processorQueue.registerSystem(new physics::IntegratorSystem(), 240, true);
    m_processorQueue.registerSystem(new physics::CollisionSystem(), 250, true);

    graphics::UISystem *ui = new graphics::UISystem();
    ui->m_gm               = &m_graphicsManager;
    m_processorQueue.registerSystem(ui, 250, true);

    setupCustomProcessors(m_processorQueue);
}

void Game::update(double dt)
{
    bedrock::Logger::touchLog(bedrock::Logger::LOG_TOUCH_UPDATE);

    float ppdt = static_cast<float>(dt);

    m_processorQueue.processAll(ppdt);
}

void Game::cleanup()
{
    resources::ResourceManager::getInstance()->processUnloads();
}

void Game::loadResources()
{
    serialization::SerializerFactory deserializationProvider
        = []() -> serialization::ISerializer * {
        serialization::YAMLSerializer *serializer
            = new serialization::YAMLSerializer();
        serializer->m_hash    = &bedrock::crc32;
        serializer->m_reverse = &ecs::ReflectionTable::Reverse;
        return serializer;
    };

    resources::ResourceManager::getInstance()->setResourceLocator(
        &m_fileResourceLocator);

    resources::ResourceManager::
        AddLoader<resources::Image, resources::PNGImage>(
            &resources::PNGImage::PNGImageLoaderFunction);

    resources::ResourceManager::AddLoader<resources::Sprite>(
        resources::Sprite::CreateSpriteLoader(),
        nullptr,
        (void *)deserializationProvider);

    resources::ResourceManager::AddLoader<resources::Template>(
        resources::Template::CreateTemplateLoader(),
        &resources::Template::TemplateUnloader,
        (void *)deserializationProvider);

    registerResouces();
}

void Game::showSplashScreen()
{
    graphics::WindowSettings splashSettings;

    resources::PNGImage splashPng
        = resources::PNGImage::fromFile(m_splashFilename, true, false);
    splashSettings.title = "Splash";
    splashSettings.dimensions
        = graphics::WindowDimensions(splashPng.m_w, splashPng.m_h);
    splashSettings.type = graphics::WindowSettings::BORDERLESS;

    graphics::IWindow *splash = m_initializer->createWindow();
    splash->setSplash(splashPng);
    splash->initialize(splashSettings);
    splash->activate();

    loadResources();

    bool shouldClose = false;
    while (!shouldClose)
    {
        splashScreenUpdate(shouldClose);
        splash->draw();
        m_initializer->handleOsStep();
    }

    splash->destroy();
}

}} // namespace perfectpixel::core
