#include "core/Game.h"

#include "core/CollisionSystem.h"
#include "core/IntegratorSystem.h"
#include "core/ResourceLoaders.h"
#include "core/Sprite.h"
#include "core/SpriteDrawSystem.h"
#include "core/Template.h"

#include "ecs/DebugProcessor.h"
#include "ecs/LifeCycleSystem.h"
#include "ecs/LifecycleComponents.h"

#include "renderer/IWindow.h"
#include "renderer/UISystem.h"

#include "serialization/YAMLSerializer.h"

#include <chrono>
#include <cstdio>
#include <thread>

#include "imgui.h"
#include "stb_image.h"

namespace perfectpixel::core {

Game::Game()
    : m_splashFilename("splash.png")
    , m_initializer(nullptr)
    , m_mainWindow(nullptr)
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

    renderer::WindowSettings mainWindowSettings;
    mainWindowSettings.title      = "Untitled";
    mainWindowSettings.dimensions = renderer::WindowDimensions(640, 480);
    mainWindowSettings.type       = renderer::WindowSettings::WINDOWED;

    PP_LOG(LEVEL_INFO, "Setting up window");

    // Virtual call, change window settings
    setupMainWindow(mainWindowSettings);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // Create the window
    m_mainWindow = createWindow(mainWindowSettings);
    m_mainWindow->activate();
    m_mainWindow->setKeyCallback(m_inputManager.getKeyCallback());
    m_mainWindow->initImGui();

    renderer::IWindow::FocusCallback focusCb;
    focusCb.m_func     = &Game::focusWrapper;
    focusCb.m_instance = this;
    m_mainWindow->setFocusCallback(focusCb);

    renderer::IWindow::SizeCallback sizeCb;
    sizeCb.m_func     = &Game::windowResizedWrapper;
    sizeCb.m_instance = this;
    m_mainWindow->setResizeCallback(sizeCb);

    PP_LOG(LEVEL_INFO, "Setting up renderer");

    m_graphicsManager.initialize();
    renderer::CameraSettings camera;
    camera.m_center    = {0, 0};
    camera.m_size      = {160, 120};
    camera.m_scaleMode = renderer::CameraSettings::SCALE_BARS;
    m_graphicsManager.setMainCamera(camera);

    ImGuiStyle &style = ImGui::GetStyle();
    ImGui::StyleColorsDark();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Fire a resize event to initialize size dependent variables
    windowResized(
        *m_mainWindow,
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

        m_initializer->handleOsStep();
        m_shouldExit = m_mainWindow->isClosed();
        if (m_shouldExit)
        {
            return;
        }

        variableUpdate(frametime.count());
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
    void *instance, renderer::IWindow &window, unsigned width, unsigned height)
{
    Game *self = reinterpret_cast<Game *>(instance);

    self->windowResized(window, width, height);
    self->variableUpdate(0.0);
}

void Game::windowResized(
    renderer::IWindow &window, unsigned width, unsigned height)
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

renderer::IWindow *Game::createWindow(const renderer::WindowSettings &settings)
{
    renderer::IWindow *window = m_initializer->createWindow();

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

    renderer::UISystem *ui = new renderer::UISystem();
    ui->m_gm               = &m_graphicsManager;
    m_processorQueue.registerSystem(ui, 250, true);

    SpriteDrawSystem *spriteDraw = new SpriteDrawSystem();
    spriteDraw->m_gm             = &m_graphicsManager;
    m_processorQueue.registerSystem(spriteDraw, 245, true);

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
    core::ResourceManager::getInstance()->processUnloads();
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

    core::ResourceManager::getInstance()->setResourceLocator(
        &m_fileResourceLocator);

    core::ResourceManager::AddLoader<renderer::ImageResource>(
        &imageLoaderFunc, &imageUnloaderFunc);

    core::ResourceManager::AddLoader<core::Sprite>(
        core::Sprite::CreateSpriteLoader(),
        nullptr,
        (void *)deserializationProvider);

    core::ResourceManager::AddLoader<core::Template>(
        core::Template::CreateTemplateLoader(),
        &core::Template::TemplateUnloader,
        (void *)deserializationProvider);

    registerResouces();
}

void Game::showSplashScreen()
{
    renderer::WindowSettings splashSettings;

    int splashW, splashH, splashChannels;
    stbi_set_flip_vertically_on_load(false);
    stbi_uc *buffer = stbi_load(
        m_splashFilename.c_str(), &splashW, &splashH, &splashChannels, 0);

    splashSettings.title      = "Splash";
    splashSettings.dimensions = renderer::WindowDimensions(splashW, splashH);
    splashSettings.type       = renderer::WindowSettings::BORDERLESS;

    renderer::IWindow *splash = m_initializer->createWindow();
    splash->setSplash(splashW, splashH, splashChannels, buffer);
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

    stbi_image_free(buffer);
}

void Game::variableUpdate(double variableDeltaT)
{
    // Handle input
    m_inputManager.update();

    m_graphicsManager.startFrame();
    m_mainWindow->startFrame();
    ImGui::NewFrame();

    static double ftBuffer[32] = {0};
    static size_t ftIndex      = 0;

    ftBuffer[ftIndex++] = variableDeltaT;
    ftIndex %= 32;

    static bool imOpen   = true;
    static bool testbool = true;
    if (ImGui::Begin("Stats", &imOpen))
    {
        double ftAvg = 0.0;
        for (size_t i = 0; i < 32; ++i)
        {
            ftAvg += ftBuffer[i];
        }
        ftAvg /= 32.0;
        double fps = 1.0 / ftAvg;
        ftAvg *= 1000.0;

        ImGui::InputDouble(
            "Frame Time",
            &ftAvg,
            0.0,
            0.0,
            "%.2f ms",
            ImGuiInputTextFlags_ReadOnly);
        ImGui::InputDouble(
            "FPS", &fps, 0.0, 0.0, "%.1f", ImGuiInputTextFlags_ReadOnly);
    }
    ImGui::End();

    // Render step
    ImGui::Render();

    bedrock::Logger::touchLog(bedrock::Logger::LOG_TOUCH_RENDER);
    m_processorQueue.renderAll(static_cast<float>(variableDeltaT));
    m_graphicsManager.drawAll(variableDeltaT);

    m_graphicsManager.endFrame(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        m_mainWindow->activate();
    }

    m_mainWindow->draw();
}

} // namespace perfectpixel::core
