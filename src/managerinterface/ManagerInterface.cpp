#include <managerinterface/ManagerInterface.h>

#include <physics/PhysicsManager.h>
#include <behaviour/BehaviourManager.h>
#include <graphics/GraphicsManager.h>
#include <input/InputManager.h>

namespace perfectpixel {
	namespace api {


		ManagerInterface::ManagerInterface()
			:  m_entityManager(nullptr)
		{
		}


		ManagerInterface::~ManagerInterface()
		{
		}

		void ManagerInterface::setEntityManager(world::EntityManager *manager)
		{
			m_entityManager = manager;
		}

		perfectpixel::world::EntityManager * ManagerInterface::Entities() const
		{
			return m_entityManager;
		}

		void ManagerInterface::setPhysicsManager(physics::PhysicsManager *manager)
		{
			m_physicsManager = manager;
		}

		perfectpixel::physics::PhysicsManager * ManagerInterface::Physics() const
		{
			return m_physicsManager;
		}

		void ManagerInterface::setBehaviourManager(behaviour::BehaviourManager *manager)
		{
			m_behaviourManager = manager;
		}

		perfectpixel::behaviour::BehaviourManager * ManagerInterface::Behaviour() const
		{
			return m_behaviourManager;
		}

		void ManagerInterface::setGraphicsManager(graphics::GraphicsManager *manager)
		{
			m_graphicsManager = manager;
		}

		perfectpixel::graphics::GraphicsManager * ManagerInterface::Graphics() const
		{
			return m_graphicsManager;
		}

		void ManagerInterface::setInputManager(input::InputManager *manager)
		{
			m_inputManager = manager;
		}

		perfectpixel::input::InputManager * ManagerInterface::Input() const
		{
			return m_inputManager;
		}

		perfectpixel::physics::TransformComponent * ManagerInterface::getTransformComponent(world::Entity entity)
		{
			return &m_physicsManager->getTransform(entity);
		}

		perfectpixel::physics::PhysicsComponent * ManagerInterface::getPhysicsComponent(world::Entity entity)
		{
			return m_physicsManager->hasPhysics(entity) ? &m_physicsManager->getPhysics(entity) : nullptr;
		}

		perfectpixel::physics::ColliderComponent * ManagerInterface::getColliderComponent(world::Entity entity)
		{
			return m_physicsManager->hasCollider(entity) ? &m_physicsManager->getCollider(entity) : nullptr;
		}

		perfectpixel::graphics::SpriteComponent * ManagerInterface::getSpriteComponent(world::Entity entity)
		{
			return m_graphicsManager->hasSprite(entity) ? &m_graphicsManager->getSprite(entity) : nullptr;
		}

	}
}
