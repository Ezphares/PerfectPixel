#include <managerinterface/ManagerInterface.h>

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

		void ManagerInterface::setEntityManager(ecs::EntityManager *manager)
		{
			m_entityManager = manager;
		}

		perfectpixel::ecs::EntityManager * ManagerInterface::Entities() const
		{
			return m_entityManager;
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

		perfectpixel::graphics::SpriteComponent * ManagerInterface::getSpriteComponent(ecs::Entity entity)
		{
			return m_graphicsManager->hasSprite(entity) ? &m_graphicsManager->getSprite(entity) : nullptr;
		}

	}
}
