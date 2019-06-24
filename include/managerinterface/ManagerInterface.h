#pragma once

#include <EntityComponentSystem/EntityManager.h>
#include <types/BitSet.h>

#include <memory>

namespace perfectpixel {

	class ManagerInterface;

	namespace behaviour
	{
		class BehaviourManager;
	}

	namespace graphics
	{
		class GraphicsManager;

		class SpriteComponent;
	}

	namespace input
	{
		class InputManager;
	}

	namespace api
	{
		class ManagerInterface
		{
		public:
			ManagerInterface();
			~ManagerInterface();

			/*
			Manager Block
			*/
			void setEntityManager(ecs::EntityManager *manager);
			ecs::EntityManager *Entities() const;

			void setBehaviourManager(behaviour::BehaviourManager *manager);
			behaviour::BehaviourManager *Behaviour() const;

			void setGraphicsManager(graphics::GraphicsManager *manager);
			graphics::GraphicsManager *Graphics() const;

			void setInputManager(input::InputManager *manager);
			input::InputManager *Input() const;

			/**
			Component Block
			*/
			// Default to behaviours
			template<typename T>
			T *getComponent(ecs::Entity entity)
			{
				return m_behaviourManager->getBehaviour<T>(ecs::Entity entity);
			}

			template<typename T>
			types::BitSet componentMask(const types::BitSet &reference) const
			{
				return m_behaviourManager->getMask<T>(const types::BitSet &reference);
			}

			graphics::SpriteComponent *getSpriteComponent(ecs::Entity entity);
			template<> graphics::SpriteComponent *getComponent<graphics::SpriteComponent>(ecs::Entity entity) { return getSpriteComponent(entity); }

		private:
			ecs::EntityManager *m_entityManager;
			behaviour::BehaviourManager *m_behaviourManager;
			graphics::GraphicsManager *m_graphicsManager;
			input::InputManager *m_inputManager;
		};
	}
}
