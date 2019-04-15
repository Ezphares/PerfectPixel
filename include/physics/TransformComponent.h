#pragma once

#include <types/vectors.h>
#include <EntityComponentSystem/Entity.h>
#include <EntityComponentSystem/Component.h>

namespace perfectpixel
{
	namespace physics
	{
		Component(Transform)
		{
			types::Vector3 m_position, m_velocity;

			ComponentConstructor(Transform)(types::Vector3 position = types::Vector3(), types::Vector3 velocity = types::Vector3())
				: m_position(position)
				, m_velocity(velocity)
			{}
		};

		FinalizeComponent(Transform);


		class TransformComponent
		{
		public:
			TransformComponent(
				ecs::Entity entity,
				types::Vector3 position = types::Vector3(),
				types::Vector3 velocity = types::Vector3());
			~TransformComponent();

		public:
			inline ecs::Entity getEntity() { return m_entity; }

		public:
			types::Vector3 m_position; // Z is depth
			types::Vector3 m_velocity;

		private:
			const ecs::Entity m_entity;
		};
	}
}
