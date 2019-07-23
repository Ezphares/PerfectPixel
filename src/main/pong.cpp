#include <graphics/LocalGL.h>
#include <graphics/UITextComponent.h>
#include <enginecore/Game.h>
#include <graphics/IWindow.h>
#include <graphics/Texture.h>
#include <EntityComponentSystem/TransformComponent.h>
#include <EntityComponentSystem/Component.h>
#include <Bedrock/vectors.h>
#include <Bedrock/TypeReflection.h>
#include <physics/PhysicsComponent.h>
#include <physics/ColliderComponent.h>
#include <serialization/YAMLSerializer.h>

#include <chrono>
#include <thread>
#include <sstream>

using namespace perfectpixel;
using namespace perfectpixel::ecs;

class BatComponent
	: public Component<BatComponent>
	, public LinearScanComponentStorage
{
public:
	PPField(BatComponent, float, MaxSpeed);
	PPField(BatComponent, float, CurrentDirection);
};

class PlayerComponent
	: public Component<PlayerComponent>
	, public MapComponentStorage
{
public:
	PPField(PlayerComponent, int32_t, InputAxis);
};

class AIComponent
	: public Component<AIComponent>
	, public MapComponentStorage
{
public:
	PPField(AIComponent, Entity, BallToTrack);
};

class BallComponent
	: public Component<BallComponent>
	, public MapComponentStorage
{
public:
	PPField(BallComponent, float, DeltaXPrev);
	PPField(BallComponent, int32_t, Score1);
	PPField(BallComponent, int32_t, Score2);

	static void Reset(Entity entity)
	{
		bedrock::Vector3 &position = TransformComponent::Position(entity);
		position = { 0, 0, position.z() };

		bedrock::Vector3 &velocity = TransformComponent::Velocity(entity);

		velocity.x() = velocity.x() > 0 ? -40.0f : 40.0f;
		velocity.y() = velocity.y() > 0 ? 35.0f : -35.0f;
		BallComponent::DeltaXPrev(entity) = velocity.x();
	}
};

class ScoreUIComponent
	: public Component<ScoreUIComponent>
	, public LinearScanComponentStorage
{
public:
	PPField(ScoreUIComponent, Entity, BallToTrack);
	PPField(ScoreUIComponent, int32_t, PlayerIndex);
};

class BatProcessor : public Processor
{
	typedef QueryHelper<With< BatComponent, TransformComponent>> BatQuery;
public:
	BatProcessor()
		: Processor(BatQuery::build())
	{}

	virtual void onCreate(const std::vector<Entity> &entities)
	{
		for (auto entity : entities)
		{
			BatComponent::MaxSpeed(entity) = 35.0f;
			BatComponent::CurrentDirection(entity) = 0.0f;
		}
	}

	virtual void onUpdate(const std::vector<Entity> &entities, float deltaT)
	{
	    for (auto entity : entities)
		{
			TransformComponent::Velocity(entity) = 
				bedrock::Vector3::UP *
				BatComponent::MaxSpeed(entity) * 
				BatComponent::CurrentDirection(entity);
		}
	}
};

class PlayerProcessor : public Processor
{
	typedef QueryHelper<With< PlayerComponent, BatComponent >> PlayerQuery;
public:
	PlayerProcessor()
		: Processor(PlayerQuery::build())
	{}

	virtual void onUpdate(const std::vector<Entity> &entities, float deltaT)
	{
		for (auto entity : entities)
		{
			BatComponent::CurrentDirection(entity) =
				m_Input->getAxisState(PlayerComponent::InputAxis(entity));
		}
	}

	// TODO:
	static input::InputManager *m_Input;
};
input::InputManager *PlayerProcessor::m_Input = nullptr;

class AIProcessor : public Processor
{
	typedef QueryHelper<With< AIComponent, BatComponent >> AIQuery;
public:
	AIProcessor()
		: Processor(AIQuery::build())
	{}

	virtual void onUpdate(const std::vector<Entity> &entities, float deltaT)
	{
		for (auto entity : entities)
		{
			BatComponent::CurrentDirection(entity) =
				TransformComponent::Position(entity).y() <
				TransformComponent::Position(AIComponent::BallToTrack(entity)).y() ?
				1.0f : -1.0f;
		}
	}
};

class BallProcessor : public Processor
{
	typedef QueryHelper<With< BallComponent, TransformComponent>> BallQuery;
public:
	BallProcessor() 
		: Processor(BallQuery::build())
	{}

	virtual void onCreate(const std::vector<Entity> &entities)
	{
		for (auto entity : entities)
		{
			BallComponent::Reset(entity);
		}
	}

	virtual void onUpdate(const std::vector<Entity> &entities, float deltaT)
	{
		for (auto entity : entities)
		{
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

			bedrock::Vector3 &velocity = TransformComponent::Velocity(entity);
			// Speed up after batting
			if (BallComponent::DeltaXPrev(entity) > 0.0f != velocity.x() > 0.0f)
			{
				velocity *= 1.05f;
			}

			BallComponent::DeltaXPrev(entity) = velocity.x();
		}
	}
};

class ScoreUIProcessor : public Processor
{
	typedef QueryHelper<With<ScoreUIComponent, graphics::UITextComponent>> ScoreUIQuery;
public:
	ScoreUIProcessor() : Processor(ScoreUIQuery::build()) {};

	virtual void onUpdate(const std::vector<Entity> &entities, float deltaT)
	{
		for (auto entity : entities)
		{
			std::stringstream textStream;
			textStream <<
				((ScoreUIComponent::PlayerIndex(entity) == 1)
				? BallComponent::Score1(ScoreUIComponent::BallToTrack(entity))
				: BallComponent::Score2(ScoreUIComponent::BallToTrack(entity)));

			graphics::UITextComponent::Text(entity) = textStream.str();
		}
	}
};

class Pong : public core::Game
{
	ecs::Entity m_ball;

	virtual void preInit()
	{
	}

	virtual void setupMainWindow(graphics::WindowSettings &mainWindow)
	{
		mainWindow.dimensions = graphics::WindowDimensions(640, 480);
		mainWindow.title = "Pong";
	}

	void createBat(float x, graphics::Sprite *spr, bool isAi)
	{
		Entity e{ EntityManager::getInstance()->create() };

		BatComponent::Register(e);

		TransformComponent::Register(e);
		TransformComponent::Position(e) =bedrock:: Vector3::RIGHT * x;

		graphics::SpriteComponent sprite{ e, spr,{ 4, 16 },{ -2,  -8 }, 1 };
		m_graphicsManager.registerSprite(e, sprite);

		physics::PhysicsComponent::Register(e);
		physics::PhysicsComponent::Bounciness(e) = 0.0f;
		physics::PhysicsComponent::Mass(e) = 1.0f;
		physics::PhysicsComponent::SimulationType(e) = physics::PhysicsComponent::FULL;

		physics::ColliderComponent::Register(e);
		physics::ColliderComponent::SetMaskRectangle(e, bedrock::AARectangle({ 4, 16 }));

		if (!isAi)
		{
			PlayerComponent::Register(e);
			PlayerComponent::InputAxis(e) = m_inputManager.lookupAxis("Vertical");
		}
		else
		{
			AIComponent::Register(e);
			AIComponent::BallToTrack(e) = m_ball;
		}
	}

	virtual void setupCustomProcessors(ecs::ProcessorQueue &queue)
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

		queue.registerProcessor(new PlayerProcessor(), 10, true);
		queue.registerProcessor(new AIProcessor(), 10, true);
		queue.registerProcessor(new BatProcessor(), 15, true);
		queue.registerProcessor(new BallProcessor(), 15, true);
		queue.registerProcessor(new ScoreUIProcessor(), 15, true);

		PlayerProcessor::m_Input = &m_inputManager;
	}

	virtual void gameStart()
	{

		ecs::Entity
			eTopWall{ EntityManager::getInstance()->create() },
			eBottomWall{ EntityManager::getInstance()->create() };

		m_ball = EntityManager::getInstance()->create();
		BallComponent::Register(m_ball);

		TransformComponent::Register(m_ball);
		TransformComponent::Register(eTopWall);
		TransformComponent::Register(eBottomWall);


		TransformComponent::Position(eTopWall) = bedrock::Vector3::UP * 58.0f;
		TransformComponent::Position(eBottomWall) = bedrock::Vector3::DOWN * 58.0f;

		graphics::Texture *tex = new  graphics::Texture(graphics::PNG::fromFile("pong-all.png") );

		graphics::Sprite *sprPlayer1 = new graphics::Sprite(
			tex,
			{ 0, .5f },
			{ .125f, .5f });
		
		graphics::Sprite *sprPlayer2 = new graphics::Sprite(
			tex,
			{ .125f, .5f },
			{ .125f, .5f });

		graphics::Sprite *sprBall = new graphics::Sprite(
			tex,
			{ .25f, .875f },
			{ .125f, .125f });

		graphics::Sprite *sprBlock = new graphics::Sprite(
			tex,
			{ .25f, .5f },
			{ .25f, .25f });

		graphics::SpriteComponent sprComBall{ m_ball, sprBall,{ 4, 4 },{ -2, -2 }, 1 };
		graphics::SpriteComponent sprComTop{ eTopWall, sprBlock,{ 160, 4 },{ -80, -2 }, 1 };
		graphics::SpriteComponent sprComBottom{ eBottomWall, sprBlock, {160, 4 }, { -80, -2}, 1 };

		m_graphicsManager.registerSprite(m_ball, sprComBall);
		m_graphicsManager.registerSprite(eTopWall, sprComTop);
		m_graphicsManager.registerSprite(eBottomWall, sprComBottom);

		physics::ColliderComponent::Register(m_ball);
		physics::ColliderComponent::SetMaskRectangle(m_ball, bedrock::AARectangle({ 4, 4 }));

		physics::ColliderComponent::Register(eTopWall);
		physics::ColliderComponent::SetMaskRectangle(eTopWall, bedrock::AARectangle({ 160, 4 }));

		physics::ColliderComponent::Register(eBottomWall);
		physics::ColliderComponent::SetMaskRectangle(eBottomWall, bedrock::AARectangle({ 160, 4 }));

		physics::PhysicsComponent::Register(m_ball);
		physics::PhysicsComponent::Bounciness(m_ball) = 1.0f;
		physics::PhysicsComponent::Mass(m_ball) = 0.0f;
		physics::PhysicsComponent::SimulationType(m_ball) = physics::PhysicsComponent::FULL;

		physics::PhysicsComponent::Register(eTopWall);
		physics::PhysicsComponent::MakeStaticCollider(eTopWall);

		physics::PhysicsComponent::Register(eBottomWall);
		physics::PhysicsComponent::MakeStaticCollider(eBottomWall);

		createBat(-78, sprPlayer1, false);
		createBat(78, sprPlayer2, true);

		// Serialization debug
		serialization::YAMLSerializer yaml;
		yaml.m_reverse = &ecs::FieldTable::Reverse;
		ecs::FieldTable::getInstance()->serialize(yaml, m_ball);
		spawnTemplate(yaml.dump());
		

		for (int32_t i = 0; i < 2; ++i)
		{
			ecs::Entity ui{ EntityManager::getInstance()->create() };

			TransformComponent::Register(ui);
			TransformComponent::Position(ui) = bedrock::Vector3(i == 0 ? -40.0f : 40.0f, -60.0f, -1.0f);

			graphics::UITextComponent::Register(ui);
			graphics::UITextComponent::Text(ui) = "0";
			graphics::UITextComponent::Alignment(ui) = graphics::UIUtil::UITA_CENTER | graphics::UIUtil::UITA_TOP;

			ScoreUIComponent::Register(ui);
			ScoreUIComponent::BallToTrack(ui) = m_ball;
			ScoreUIComponent::PlayerIndex(ui) = i + 1;
		}
	}

	virtual void step()
	{
		if (m_inputManager.isButtonDown("Info"))
		{
			MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);
		}

	}

	virtual void splashScreenUpdate(bool &closeSplash)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		closeSplash = true;
	}
};

PP_GAME(Pong);
