#include "core/Game.h"

#include "ecs/Component.h"
#include "ecs/Context.h"
#include "ecs/QuerySystem.h"
#include "ecs/TransformComponent.h"

#include "renderer/IWindow.h"
#include "renderer/LocalGL.h"
#include "renderer/Texture.h"

#include "core/ColliderComponent.h"
#include "core/PhysicsComponent.h"
#include "core/SpriteComponent.h"
#include "core/UI/UITextComponent.h"

#include "bedrock/TypeReflection.h"
#include "bedrock/vectors.h"

#include "serialization/YAMLSerializer.h"

#include "core/Resource.h"
#include "core/Sprite.h"
#include "core/Template.h"

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
        position                   = {0, 0, position.z};

        bedrock::Vector3 &velocity
            = physics::PhysicsComponent::Velocity(entity);

        velocity.x                        = velocity.x > 0 ? -40.0f : 40.0f;
        velocity.y                        = velocity.y > 0 ? 35.0f : -35.0f;
        BallComponent::DeltaXPrev(entity) = velocity.x;
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

    static void onCreate(std::span<Entity> entities)
    {
        for (auto entity : entities)
        {
            BatComponent::MaxSpeed(entity)         = 35.0f;
            BatComponent::CurrentDirection(entity) = 0.0f;
        }
    }

    static void onUpdate(std::span<Entity> entities, float deltaT)
    {
        (void)deltaT;
        for (auto entity : entities)
        {
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

    static void onUpdate(std::span<Entity> entities, float deltaT)
    {
        (void)deltaT;
        for (auto entity : entities)
        {
            BatComponent::CurrentDirection(entity)
                = m_Input->getAxisState(PlayerComponent::InputAxis(entity));
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

    static void onUpdate(std::span<Entity> entities, float deltaT)
    {
        (void)deltaT;
        for (auto entity : entities)
        {
            BatComponent::CurrentDirection(entity)
                = TransformComponent::Position(entity).y
                          < TransformComponent::Position(
                                AIComponent::BallToTrack(entity))
                                .y
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

    static void onCreate(std::span<Entity> entities)
    {
        for (auto entity : entities)
        {
            BallComponent::Reset(entity);
        }
    }

    static void onUpdate(std::span<Entity> entities, float deltaT)
    {
        (void)deltaT;
        for (auto entity : entities)
        {
            bool shouldReset = false;

            if (TransformComponent::Position(entity).x < -80)
            {
                BallComponent::Score2(entity)++;
                shouldReset = true;
            }
            else if (TransformComponent::Position(entity).x > 80)
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
            if (BallComponent::DeltaXPrev(entity) > 0.0f != velocity.x > 0.0f)
            {
                velocity *= 1.05f;
            }

            BallComponent::DeltaXPrev(entity) = velocity.x;
        }
    }
};

class ScoreUISystem : public QuerySystem
{
    typedef QueryHelper<With<ScoreUIComponent, core::UITextComponent>>
        ScoreUIQuery;

public:
    ScoreUISystem()
        : QuerySystem(ScoreUIQuery::build())
    {
        m_onUpdate = &onUpdate;
    };

    static void onUpdate(std::span<Entity> entities, float deltaT)
    {
        (void)deltaT;
        for (auto entity : entities)
        {
            std::stringstream textStream;
            textStream
                << ((ScoreUIComponent::PlayerIndex(entity) == 1)
                        ? BallComponent::Score1(
                            ScoreUIComponent::BallToTrack(entity))
                        : BallComponent::Score2(
                            ScoreUIComponent::BallToTrack(entity)));

            core::UITextComponent::Text(entity) = textStream.str();
        }
    }
};

class Pong : public core::Game
{
    ecs::Entity m_ball;

    virtual void preInit()
    {}

    virtual void setupMainWindow(renderer::WindowSettings &mainWindow)
    {
        mainWindow.dimensions = renderer::WindowDimensions(640, 480);
        mainWindow.title      = "Pong";
    }

    void createBat(float x, core::Sprite *spr, bool isAi)
    {
        Entity e{EntityManager::getInstance()->create()};

        BatComponent::Register(e);

        TransformComponent::Register(e);
        TransformComponent::Position(e) = bedrock::Vector3::RIGHT * x;

        core::SpriteComponent::Register(e);
        core::SpriteComponent::SpriteData(e) = *spr;
        core::SpriteComponent::Size(e)       = {4, 16};
        core::SpriteComponent::Offset(e)     = {-2, -8};
        core::SpriteComponent::FPS(e)        = 1.0f;

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
        core::Resource ballTpl = core::Resource(
            bedrock::typeID<core::Template>(), PP_ID(ball.tpl));

        m_ball = ballTpl.get<core::Template>()->spawn();

        core::Resource wallTpl = core::Resource(
            bedrock::typeID<core::Template>(), PP_ID(wall.tpl));
        ecs::Entity eTopWall{wallTpl.get<core::Template>()->spawn()},
            eBottomWall{wallTpl.get<core::Template>()->spawn()};

        TransformComponent::Position(eTopWall) = bedrock::Vector3::UP * 58.0f;
        TransformComponent::Position(eBottomWall)
            = bedrock::Vector3::DOWN * 58.0f;

        core::Resource img = core::Resource(
            bedrock::typeID<renderer::ImageResource>(), PP_ID(pong_all.png));

        core::Sprite *sprPlayer1
            = new core::Sprite(img, {0, .5f}, {.125f, .5f});

        core::Sprite *sprPlayer2
            = new core::Sprite(img, {.125f, .5f}, {.125f, .5f});

        createBat(-78, sprPlayer1, false);
        createBat(78, sprPlayer2, true);

        for (int32_t i = 0; i < 2; ++i)
        {
            ecs::Entity ui{EntityManager::getInstance()->create()};

            TransformComponent::Register(ui);
            TransformComponent::Position(ui)
                = bedrock::Vector3(i == 0 ? -40.0f : 40.0f, -60.0f, -1.0f);

            core::UITextComponent::Register(ui);
            core::UITextComponent::Text(ui) = "0";
            core::UITextComponent::Alignment(ui)
                = core::UIUtil::UITA_CENTER | core::UIUtil::UITA_TOP;

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
        auto levelBundle = []() {
            bedrock::UniqueVoidPtr res = bedrock::UniqueVoidPtr::create<
                renderer::ImageResourceUserData>();
            res.get<renderer::ImageResourceUserData>().bundleID = 2;
            return std::move(res);
        };

        // TEXTURES
        core::ResourceManager::RegisterResource(
            "pong_all.png",
            core::ResourceManager::RLS_AUTO_USE,
            PP_ID(pong_all.png),
            bedrock::typeID<renderer::ImageResource>(),
            0,
            levelBundle());

        // SPRITES
        core::ResourceManager::RegisterResource(
            "ball.spr",
            core::ResourceManager::RLS_AUTO_REF,
            PP_ID(ball.spr),
            bedrock::typeID<core::Sprite>());

        core::ResourceManager::RegisterResource(
            "wall.spr",
            core::ResourceManager::RLS_AUTO_REF,
            PP_ID(wall.spr),
            bedrock::typeID<core::Sprite>());

        // TEMPLATES
        core::ResourceManager::RegisterResource(
            "ball.tpl",
            core::ResourceManager::RLS_AUTO_USE,
            PP_ID(ball.tpl),
            bedrock::typeID<core::Template>());

        core::ResourceManager::RegisterResource(
            "wall.tpl",
            core::ResourceManager::RLS_AUTO_USE,
            PP_ID(wall.tpl),
            bedrock::typeID<core::Template>());
    }
};

PP_GAME(Pong);
