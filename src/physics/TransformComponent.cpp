#include <physics/TransformComponent.h>

namespace perfectpixel
{
	namespace physics
	{

		TransformComponent::TransformComponent(
			ecs::Entity entity,
			types::Vector3 position,
			types::Vector3 velocity)
			: m_entity(entity)
			, m_position(position)
			, m_velocity(velocity)
		{
		}


		TransformComponent::~TransformComponent()
		{
		}

	}
}
