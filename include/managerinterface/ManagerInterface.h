#pragma once

#include <EntityComponentSystem/EntityManager.h>
#include <types/BitSet.h>

#include <memory>

namespace perfectpixel {

	class ManagerInterface;

	namespace physics
	{
		class PhysicsManager;

		class TransformComponent;
		class PhysicsComponent;
		class ColliderComponent;
	}

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
			void setEntityManager(world::EntityManager *manager);
			world::EntityManager *Entities() const;

			void setPhysicsManager(physics::PhysicsManager *manager);
			physics::PhysicsManager *Physics() const;

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
			T *getComponent(world::Entity entity)
			{
				return m_behaviourManager->getBehaviour<T>(world::Entity entity);
			}

			template<typename T>
			types::BitSet componentMask(const types::BitSet &reference) const
			{
				return m_behaviourManager->getMask<T>(const types::BitSet &reference);
			}

			physics::TransformComponent *getTransformComponent(world::Entity entity);
			template<> physics::TransformComponent *getComponent<physics::TransformComponent>(world::Entity entity) { return getTransformComponent(entity); }

			physics::PhysicsComponent *getPhysicsComponent(world::Entity entity);
			template<> physics::PhysicsComponent *getComponent<physics::PhysicsComponent>(world::Entity entity) { return getPhysicsComponent(entity); }

			physics::ColliderComponent *getColliderComponent(world::Entity entity);
			template<> physics::ColliderComponent *getComponent<physics::ColliderComponent>(world::Entity entity) { return getColliderComponent(entity); }

			graphics::SpriteComponent *getSpriteComponent(world::Entity entity);
			template<> graphics::SpriteComponent *getComponent<graphics::SpriteComponent>(world::Entity entity) { return getSpriteComponent(entity); }

		private:
			world::EntityManager *m_entityManager;
			physics::PhysicsManager *m_physicsManager;
			behaviour::BehaviourManager *m_behaviourManager;
			graphics::GraphicsManager *m_graphicsManager;
			input::InputManager *m_inputManager;
		};
	}
}
