#include <physics/ColliderComponent.h>

#include <EntityComponentSystem/EntityManager.h>

#include <types/PpException.h>

namespace perfectpixel {
	namespace physics {

		void ColliderComponent::setMaskRectangle(ecs::Entity entity, const types::AARectangle &rectangle)
		{
			ColliderComponent::MaskType(entity) = RECTANGLE;
			ColliderComponent::Mask(entity).m_rectangle = rectangle;
		}

		types::AARectangle ColliderComponent::getMaskRectangle(ecs::Entity entity)
		{
			if (ColliderComponent::MaskType(entity) != RECTANGLE)
			{
				types::PpException("Invalid mask type");
			}
			return ColliderComponent::Mask(entity).m_rectangle;
		}

		void ColliderComponent::setMaskCircle(ecs::Entity entity, const types::Circle &circle)
		{
			ColliderComponent::MaskType(entity) = CIRCLE;
			ColliderComponent::Mask(entity).m_circle = circle;
		}

		const perfectpixel::types::Circle ColliderComponent::getMaskCircle(ecs::Entity entity)
		{
			if (ColliderComponent::MaskType(entity) != CIRCLE)
			{
				types::PpException("Invalid mask type");
			}
			return ColliderComponent::Mask(entity).m_circle;
		}

		void ColliderComponent::GetNear(ecs::EntityManager *entityManager, std::vector<ecs::Entity> &toCheck, const types::Vector2 &point)
		{
			types::BitSet mask = entityManager->all();
			Filter(mask, IComponentStorage::WITH);
			entityManager->expandMask(mask, &toCheck, nullptr);
		}

	}
}
