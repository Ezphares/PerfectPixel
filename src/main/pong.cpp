#include <enginecore/Game.h>

#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/QuerySystem.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <graphics/IWindow.h>
#include <graphics/LocalGL.h>
#include <graphics/SpriteComponent.h>
#include <graphics/Texture.h>
#include <graphics/UITextComponent.h>

#include <physics/ColliderComponent.h>
#include <physics/PhysicsComponent.h>

#include <Bedrock/TypeReflection.h>
#include <Bedrock/vectors.h>

#include <serialization/YAMLSerializer.h>

#include <Resources/PNGImage.h>
#include <Resources/Resource.h>
#include <Resources/Sprite.h>
#include <Resources/Template.h>

#include <chrono>
#include <sstream>
#include <thread>

using namespace perfectpixel;
using namespace perfectpixel::ecs;

class BatComponent : public Component<BatComponent, LinearScanComponentStorage>
{
public:
    PPField(BatComponent, float, MaxSpeed);
    PPField(BatComponent, float, CurrentDirection);
};

class PlayerComponent : public Component<PlayerComponent, MapComponentStorage>
{
public:
    PPField(PlayerComponent, int32_t, InputAxis);
};

class AIComponent : public Component<AIComponent, MapComponentStorage>
{
public:
    PPField(AIComponent, Entity, BallToTrack);
};

class BallComponent : public Component<BallComponent, MapComponentStorage>
{
public:
    PPField(BallComponent, float, DeltaXPrev);
    PPField(BallComponent, int32_t, Score1);
    PPField(BallComponent, int32_t, Score2);

    static void Reset(Entity entity)
    {
        bedrock::Vector3 &position = TransformComponent::Position(entity);
        position                   = {0, 0, position.z()};

        bedrock::Vector3 &velocity
            = physics::PhysicsComponent::Velocity(entity);

        velocity.x()                      = velocity.x() > 0 ? -40.0f : 40.0f;
        velocity.y()                      = velocity.y() > 0 ? 35.0f : -35.0f;
        BallComponent::DeltaXPrev(entity) = velocity.x();
    }
};

class ScoreUIComponent
    : public Component<ScoreUIComponent, LinearScanComponentStorage>
{
public:
    PPField(ScoreUIComponent, Entity, BallToTrack);
    PPField(ScoreUIComponent, int32_t, PlayerIndex);
};

class BatSystem : public QuerySystem
{
    typedef QueryHelper<With<BatComponent, TransformComponent>> BatQuery;

public:
    BatSystem()
        : QuerySystem(BatQuery::build())
    {
        m_onCreate = &onCreate;
        m_onUpdate = &onUpdate;
    }

    static void onCreate(const RangeLimit &begin, const RangeLimit &end)
    {
        for (auto it = begin; it != end; ++it)
        {
            BatComponent::MaxSpeed(*it)         = 35.0f;
            BatComponent::CurrentDirection(*it) = 0.0f;
        }
    }

    static void
    onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT)
    {
        (void)deltaT;
        for (auto it = begin; it != end; ++it)
        {
            Entity entity = *it;
            physics::PhysicsComponent::Velocity(entity)
                = bedrock::Vector3::UP * BatComponent::MaxSpeed(entity)
                  * BatComponent::CurrentDirection(entity);
        }
    }
};

class PlayerSystem : public QuerySystem
{
    typedef QueryHelper<With<PlayerComponent, BatComponent>> PlayerQuery;

public:
    PlayerSystem()
        : QuerySystem(PlayerQuery::build())
    {
        m_onUpdate = &onUpdate;
    }

    static void
    onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT)
    {
        (void)deltaT;
        for (auto it = begin; it != end; ++it)
        {
            BatComponent::CurrentDirection(*it)
                = m_Input->getAxisState(PlayerComponent::InputAxis(*it));
        }
    }

    // TODO:
    static input::InputManager *m_Input;
};
input::InputManager *PlayerSystem::m_Input = nullptr;

class AISystem : public QuerySystem
{
    typedef QueryHelper<With<AIComponent, BatComponent>> AIQuery;

public:
    AISystem()
        : QuerySystem(AIQuery::build())
    {
        m_onUpdate = &onUpdate;
    }

    static void
    onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT)
    {
        (void)deltaT;
        for (auto it = begin; it != end; ++it)
        {
            Entity entity = *it;
            BatComponent::CurrentDirection(entity)
                = TransformComponent::Position(entity).y()
                          < TransformComponent::Position(
                                AIComponent::BallToTrack(entity))
                                .y()
                      ? 1.0f
                      : -1.0f;
        }
    }
};

class BallSystem : public QuerySystem
{
    typedef QueryHelper<With<BallComponent, TransformComponent>> BallQuery;

public:
    BallSystem()
        : QuerySystem(BallQuery::build())
    {
        m_onCreate = &onCreate;
        m_onUpdate = &onUpdate;
    }

    static void onCreate(const RangeLimit &begin, const RangeLimit &end)
    {
        for (auto it = begin; it != end; ++it)
        {
            BallComponent::Reset(*it);
        }
    }

    static void
    onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT)
    {
        (void)deltaT;
        for (auto it = begin; it != end; ++it)
        {
            Entity entity    = *it;
            bool shouldReset = false;

            if (TransformComponent::Position(entity).x() < -80)
            {
                BallComponent::Score2(entity)++;
                shouldReset = true;
            }
            else if (TransformComponent::Position(entity).x() > 80)
            {
                BallComponent::Score1(entity)++;
                shouldReset = true;
            }

            if (shouldReset)
            {
                BallComponent::Reset(entity);
            }

            bedrock::Vector3 &velocity
                = physics::PhysicsComponent::Velocity(entity);
            // Speed up after batting
            if (BallComponent::DeltaXPrev(entity) > 0.0f != velocity.x() > 0.0f)
            {
                velocity *= 1.05f;
            }

            BallComponent::DeltaXPrev(entity) = velocity.x();
        }
    }
};

class ScoreUISystem : public QuerySystem
{
    typedef QueryHelper<With<ScoreUIComponent, graphics::UITextComponent>>
        ScoreUIQuery;

public:
    ScoreUISystem()
        : QuerySystem(ScoreUIQuery::build())
    {
        m_onUpdate = &onUpdate;
    };

    static void
    onUpdate(const RangeLimit &begin, const RangeLimit &end, float deltaT)
    {
        (void)deltaT;
        for (auto it = begin; it != end; ++it)
        {
            Entity entity = *it;
            std::stringstream textStream;
            textStream
                << ((ScoreUIComponent::PlayerIndex(entity) == 1)
                        ? BallComponent::Score1(
                            ScoreUIComponent::BallToTrack(entity))
                        : BallComponent::Score2(
                            ScoreUIComponent::BallToTrack(entity)));

            graphics::UITextComponent::Text(entity) = textStream.str();
        }
    }
};

class Pong : public core::Game
{
    ecs::Entity m_ball;

    virtual void preInit()
    {}

    virtual void setupMainWindow(graphics::WindowSettings &mainWindow)
    {
        mainWindow.dimensions = graphics::WindowDimensions(640, 480);
        mainWindow.title      = "Pong";
    }

    void createBat(float x, resources::Sprite *spr, bool isAi)
    {
        Entity e{EntityManager::getInstance()->create()};

        BatComponent::Register(e);

        TransformComponent::Register(e);
        TransformComponent::Position(e) = bedrock::Vector3::RIGHT * x;

        graphics::SpriteComponent::Register(e);
        graphics::SpriteComponent::SpriteData(e) = *spr;
        graphics::SpriteComponent::Size(e)       = {4, 16};
        graphics::SpriteComponent::Offset(e)     = {-2, -8};
        graphics::SpriteComponent::FPS(e)        = 1.0f;

        physics::PhysicsComponent::Register(e);
        physics::PhysicsComponent::Bounciness(e)     = 0.0f;
        physics::PhysicsComponent::Mass(e)           = 1.0f;
        physics::PhysicsComponent::SimulationType(e) = physics::PST_FULL;

        physics::ColliderComponent::Register(e);
        physics::ColliderComponent::SetMaskRectangle(
            e, bedrock::AARectangle({2, 8}));

        if (!isAi)
        {
            PlayerComponent::Register(e);
            PlayerComponent::InputAxis(e)
                = m_inputManager.lookupAxis("Vertical");
        }
        else
        {
            AIComponent::Register(e);
            AIComponent::BallToTrack(e) = m_ball;
        }

        // Serialization debug
        serialization::YAMLSerializer yaml;
        yaml.m_reverse = &ecs::ReflectionTable::Reverse;
        ecs::ReflectionTable::getInstance()->serialize(yaml, e);
        yaml.dump();
    }

    virtual void setupCustomProcessors(ecs::SystemQueue &queue)
    {

        m_inputManager.registerButton("Left");
        m_inputManager.registerButton("Right");
        m_inputManager.registerButton("Up");
        m_inputManager.registerButton("Down");
        m_inputManager.registerButton("Jump");
        m_inputManager.registerButton("Info");

        m_inputManager.registerAxis("Horizontal");
        m_inputManager.registerAxis("Vertical");

        m_inputManager.bindAxisToButtons("Horizontal", "Left", "Right");
        m_inputManager.bindAxisToButtons("Vertical", "Down", "Up");

        m_inputManager.bindButton("Left", VK_LEFT);
        m_inputManager.bindButton("Right", VK_RIGHT);
        m_inputManager.bindButton("Up", VK_UP);
        m_inputManager.bindButton("Down", VK_DOWN);
        m_inputManager.bindButton("Jump", 0x5A); // Z
        m_inputManager.bindButton("Info", VK_F1);

        queue.registerSystem(new PlayerSystem(), 10, true);
        queue.registerSystem(new AISystem(), 10, true);
        queue.registerSystem(new BatSystem(), 15, true);
        queue.registerSystem(new BallSystem(), 15, true);
        queue.registerSystem(new ScoreUISystem(), 15, true);

        PlayerSystem::m_Input = &m_inputManager;
    }

    virtual void gameStart()
    {
        resources::Resource ballTpl = resources::Resource(
            bedrock::typeID<resources::Template>(), PP_ID(ball.tpl));

        m_ball = ballTpl.get<resources::Template>()->spawn();

        resources::Resource wallTpl = resources::Resource(
            bedrock::typeID<resources::Template>(), PP_ID(wall.tpl));
        ecs::Entity eTopWall{wallTpl.get<resources::Template>()->spawn()},
            eBottomWall{wallTpl.get<resources::Template>()->spawn()};

        TransformComponent::Position(eTopWall) = bedrock::Vector3::UP * 58.0f;
        TransformComponent::Position(eBottomWall)
            = bedrock::Vector3::DOWN * 58.0f;

        resources::Resource img = resources::Resource(
            bedrock::typeID<resources::Image>(), PP_ID(pong_all.png));

        resources::Sprite *sprPlayer1
            = new resources::Sprite(img, {0, .5f}, {.125f, .5f});

        resources::Sprite *sprPlayer2
            = new resources::Sprite(img, {.125f, .5f}, {.125f, .5f});

        createBat(-78, sprPlayer1, false);
        createBat(78, sprPlayer2, true);

        for (int32_t i = 0; i < 2; ++i)
        {
            ecs::Entity ui{EntityManager::getInstance()->create()};

            TransformComponent::Register(ui);
            TransformComponent::Position(ui)
                = bedrock::Vector3(i == 0 ? -40.0f : 40.0f, -60.0f, -1.0f);

            graphics::UITextComponent::Register(ui);
            graphics::UITextComponent::Text(ui) = "0";
            graphics::UITextComponent::Alignment(ui)
                = graphics::UIUtil::UITA_CENTER | graphics::UIUtil::UITA_TOP;

            ScoreUIComponent::Register(ui);
            ScoreUIComponent::BallToTrack(ui) = m_ball;
            ScoreUIComponent::PlayerIndex(ui) = i + 1;
        }
    }

    virtual void step()
    {
        if (m_inputManager.isButtonDown("Info"))
        {
            MessageBoxA(
                0, (char *)glGetString(GL_VERSION), "OPENGL VERSION", 0);
        }
    }

    virtual void splashScreenUpdate(bool &closeSplash)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        closeSplash = true;
    }

    virtual void registerResouces()
    {
        // TEXTURES
        resources::ResourceManager::RegisterResource(
            "pong_all.png",
            resources::ResourceManager::RLS_AUTO_USE,
            PP_ID(pong_all.png),
            bedrock::typeID<resources::Image>(),
            bedrock::typeID<resources::PNGImage>());

        // SPRITES
        resources::ResourceManager::RegisterResource(
            "ball.spr",
            resources::ResourceManager::RLS_AUTO_REF,
            PP_ID(ball.spr),
            bedrock::typeID<resources::Sprite>());

        resources::ResourceManager::RegisterResource(
            "wall.spr",
            resources::ResourceManager::RLS_AUTO_REF,
            PP_ID(wall.spr),
            bedrock::typeID<resources::Sprite>());

        // TEMPLATES
        resources::ResourceManager::RegisterResource(
            "ball.tpl",
            resources::ResourceManager::RLS_AUTO_USE,
            PP_ID(ball.tpl),
            bedrock::typeID<resources::Template>());

        resources::ResourceManager::RegisterResource(
            "wall.tpl",
            resources::ResourceManager::RLS_AUTO_USE,
            PP_ID(wall.tpl),
            bedrock::typeID<resources::Template>());
    }
};

PP_GAME(Pong);
