#include <graphics/LocalGL.h>
#include <enginecore/Game.h>
#include <graphics/IWindow.h>
#include <graphics/Texture.h>
#include <EntityComponentSystem/TransformComponent.h>
#include <types/vectors.h>
#include <behaviour/Behaviour.h>
#include <physics/PhysicsComponent.h>
#include <physics/ColliderComponent.h>

#include <chrono>
#include <thread>

using namespace perfectpixel;

class BatBehaviour : public behaviour::Behaviour
{
public:
	BatBehaviour(
		ecs::Entity entity)
		: behaviour::Behaviour(entity)
		, m_speed(35)
	{}
	virtual ~BatBehaviour() {}

	virtual void onUpdate(types::PpFloat deltaT)
	{
		ecs::TransformComponent::Velocity(getEntity()).y() = pp()->Input()->getAxisState("Vertical") * m_speed;
	}

private:
	types::PpFloat m_speed;
};

class AIBatBehaviour : public behaviour::Behaviour
{
public:
	AIBatBehaviour(
		ecs::Entity entity,
		ecs::Entity ball)
		: Behaviour(entity)
		, m_ball(ball)
		, m_speed(35)
	{}
	virtual ~AIBatBehaviour(){}

	virtual void onCreate()
	{
	}

	virtual void onUpdate(types::PpFloat deltaT)
	{
		bool up = ecs::TransformComponent::Position(getEntity()).y() < ecs::TransformComponent::Position(m_ball).y();
		ecs::TransformComponent::Velocity(getEntity()).y() = (up ? 1.0f : -1.0f) * m_speed;
	}

private:
	ecs::Entity m_ball;
	types::PpFloat m_speed;
};

class BallBehaviour : public behaviour::Behaviour
{
public:
	BallBehaviour(ecs::Entity entity)
		: behaviour::Behaviour(entity)
	{}

	virtual ~BallBehaviour() {}

	virtual void onCreate()
	{
		reset();
	}

	void reset()
	{	
		types::Vector3 &position = ecs::TransformComponent::Position(getEntity());
		position = { 0, 0, position.z() };

		types::Vector3 &velocity = ecs::TransformComponent::Velocity(getEntity());

		velocity.x() = velocity.x() > 0 ? -40.0f : 40.0f;
		velocity.y() = velocity.y() > 0 ? 35.0f : -35.0f;
		m_dxPrev = velocity.x();
	}

	virtual void onUpdate(types::PpFloat deltaT)
	{
		bool shouldReset = false;

		if (ecs::TransformComponent::Position(getEntity()).x() < -80)
		{
			m_score2++;
			shouldReset = true;
		}
		else if (ecs::TransformComponent::Position(getEntity()).x() > 80)
		{
			m_score1++;
			shouldReset = true;
		}

		if (shouldReset)
		{
			reset();
		}
		types::Vector3 &velocity = ecs::TransformComponent::Velocity(getEntity());
		// Speed up after batting
		if (m_dxPrev > 0 != velocity.x() > 0)
		{
			velocity *= 1.05f;
		}

		m_dxPrev = velocity.x();
	}


private:
	physics::TransformComponent *m_transform;
	int m_score1, m_score2;
	types::PpFloat m_dxPrev;
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

	void createBat(types::PpFloat x, graphics::Sprite *spr, bool isAi)
	{
		ecs::Entity e{ m_entityManager.create() };
		ecs::TransformComponent::Register(e);
		ecs::TransformComponent::Position(e) =types:: Vector3::RIGHT * x;

		graphics::SpriteComponent sprite{ e, spr,{ 4, 16 },{ -2,  -8 }, 1 };
		m_graphicsManager.registerSprite(e, sprite);

		physics::PhysicsComponent::Register(e);
		physics::PhysicsComponent::Bounciness(e) = 0.0f;
		physics::PhysicsComponent::Mass(e) = 1.0f;
		physics::PhysicsComponent::SimulationType(e) = physics::PhysicsComponent::FULL;

		physics::ColliderComponent::Register(e);
		physics::ColliderComponent::setMaskRectangle(e, types::AARectangle({ 4, 16 }));

		if (!isAi)
		{
			m_behaviourManager.registerBehaviour(e, new BatBehaviour(e));
		}
		else
		{
			m_behaviourManager.registerBehaviour(e, new AIBatBehaviour(e, m_ball));
		}
	}

	virtual void gameStart()
	{
		ecs::Entity
			eTopWall{ m_entityManager.create() },
			eBottomWall{ m_entityManager.create() };

		m_ball = m_entityManager.create();

		ecs::TransformComponent::Register(m_ball);
		ecs::TransformComponent::Register(eTopWall);
		ecs::TransformComponent::Register(eBottomWall);


		ecs::TransformComponent::Position(eTopWall) = types::Vector3::UP * 58.0f;
		ecs::TransformComponent::Position(eBottomWall) = types::Vector3::DOWN * 58.0f;

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
		physics::ColliderComponent::setMaskRectangle(m_ball, types::AARectangle({ 4, 4 }));

		physics::ColliderComponent::Register(eTopWall);
		physics::ColliderComponent::setMaskRectangle(eTopWall, types::AARectangle({ 160, 4 }));

		physics::ColliderComponent::Register(eBottomWall);
		physics::ColliderComponent::setMaskRectangle(eBottomWall, types::AARectangle({ 160, 4 }));

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
		m_behaviourManager.registerBehaviour(m_ball, new BallBehaviour(m_ball));

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
