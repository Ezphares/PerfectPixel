#pragma once

#include <types/vectors.h>
#include <worldgraph/Entity.h>

namespace perfectpixel
{
	namespace physics
	{
		class TransformComponent
		{
		public:
			TransformComponent(
				world::Entity entity,
				types::Vector3 position = types::Vector3(),
				types::Vector3 velocity = types::Vector3());
			~TransformComponent();

		public:
			inline world::Entity getEntity() { return m_entity; }

		public:
			types::Vector3 m_position; // Z is depth
			types::Vector3 m_velocity;

		private:
			const world::Entity m_entity;
		};
	}
}
