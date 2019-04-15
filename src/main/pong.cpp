#include <graphics/LocalGL.h>
#include <enginecore/Game.h>
#include <graphics/IWindow.h>
#include <graphics/Texture.h>
#include <types/vectors.h>
#include <behaviour/Behaviour.h>

#include "EntityComponentSystem/ComponentRegistry.h"

#include <chrono>
#include <thread>

using namespace perfectpixel;

class BatBehaviour : public behaviour::Behaviour
{
public:
	BatBehaviour(
		world::Entity entity)
		: behaviour::Behaviour(entity)
		, m_transform(nullptr)
		, m_speed(35)
	{}
	virtual ~BatBehaviour() {}

	virtual void onCreate()
	{
		m_transform = pp()->getComponent<physics::TransformComponent>(getEntity());
	}

	virtual void onUpdate(types::PpFloat deltaT)
	{
		m_transform->m_velocity.y() = pp()->Input()->getAxisState("Vertical") * m_speed;
	}

private:
	physics::TransformComponent *m_transform;
	types::PpFloat m_speed;
};

class AIBatBehaviour : public behaviour::Behaviour
{
public:
	AIBatBehaviour(
		world::Entity entity,
		world::Entity ball)
		: Behaviour(entity)
		, m_transform(nullptr)
		, m_ballTransform(nullptr)
		, m_ball(ball)
		, m_speed(35)
	{}
	virtual ~AIBatBehaviour(){}

	virtual void onCreate()
	{
		m_transform = pp()->getComponent<physics::TransformComponent>(getEntity());
		m_ballTransform = pp()->getComponent<physics::TransformComponent>(m_ball);
	}

	virtual void onUpdate(types::PpFloat deltaT)
	{
		bool up = m_transform->m_position.y() < m_ballTransform->m_position.y();
		m_transform->m_velocity.y() = (up ? 1.0f : -1.0f) * m_speed;
	}

private:
	physics::TransformComponent *m_transform;
	physics::TransformComponent *m_ballTransform;
	world::Entity m_ball;
	types::PpFloat m_speed;
};

class BallBehaviour : public behaviour::Behaviour
{
public:
	BallBehaviour(world::Entity entity)
		: behaviour::Behaviour(entity)
	{}

	virtual ~BallBehaviour() {}

	virtual void onCreate()
	{
		m_transform = pp()->getComponent<physics::TransformComponent>(getEntity());
		reset();
	}

	void reset()
	{	
		m_transform->m_position = { 0, 0, m_transform->m_position.z() };

		m_transform->m_velocity.x() = m_transform->m_velocity.x() > 0 ? -40.0f : 40.0f;
		m_transform->m_velocity.y() = m_transform->m_velocity.y() > 0 ? 35.0f : -35.0f;
		m_dxPrev = m_transform->m_velocity.x();
	}

	virtual void onUpdate(types::PpFloat deltaT)
	{
		bool shouldReset = false;

		if (m_transform->m_position.x() < -80)
		{
			m_score2++;
			shouldReset = true;
		}
		else if (m_transform->m_position.x() > 80)
		{
			m_score1++;
			shouldReset = true;
		}

		if (shouldReset)
		{
			reset();
		}

		// Speed up after batting
		if (m_dxPrev > 0 != m_transform->m_velocity.x() > 0)
		{
			m_transform->m_velocity *= 1.05f;
		}

		m_dxPrev = m_transform->m_velocity.x();
	}


private:
	physics::TransformComponent *m_transform;
	int m_score1, m_score2;
	types::PpFloat m_dxPrev;
};

class Pong : public core::Game
{
	world::Entity m_ball;

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
		world::Entity e{ m_entityManager.create() };
		m_physicsManager.getTransform(e).m_position.x() = x;

		graphics::SpriteComponent sprite{ e, spr,{ 4, 16 },{ -2,  -8 }, 1 };
		m_graphicsManager.registerSprite(e, sprite);

		physics::PhysicsComponent physics(e, 1.0f, 0.0f, physics::PhysicsComponent::FULL);

		physics::ColliderComponent collider(e, types::AARectangle({ 4, 16 }));
		m_physicsManager.registerPhysics(physics);
		m_physicsManager.registerCollider(collider);

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
		world::Entity
			ePlayer1{ m_entityManager.create() },
			ePlayer2{ m_entityManager.create() },
			eTopWall{ m_entityManager.create() },
			eBottomWall{ m_entityManager.create() };

		m_ball = m_entityManager.create();

		m_physicsManager.getTransform(eTopWall).m_position.y() = 58;
		m_physicsManager.getTransform(eBottomWall).m_position.y() = -58;

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

		physics::ColliderComponent 
			colliderBall(m_ball, types::AARectangle({ 4, 4 })),
			colliderTop(eTopWall, types::AARectangle({ 160, 4 })),
			colliderBottom(eBottomWall, types::AARectangle({ 160, 4 }));

		physics::PhysicsComponent
			physicsBall(m_ball, 0.0f, 1.0f, physics::PhysicsComponent::FULL),
			physicsTop(physics::PhysicsComponent::staticCollider(eTopWall)),
			physicsBottom(physics::PhysicsComponent::staticCollider(eBottomWall));

		m_physicsManager.registerCollider(colliderBall);
		m_physicsManager.registerCollider(colliderTop);
		m_physicsManager.registerCollider(colliderBottom);

		m_physicsManager.registerPhysics(physicsBall);
		m_physicsManager.registerPhysics(physicsTop);
		m_physicsManager.registerPhysics(physicsBottom);

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
