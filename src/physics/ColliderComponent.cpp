#include <physics/ColliderComponent.h>

#include <EntityComponentSystem/EntityManager.h>

#include <Bedrock/PpException.h>

namespace perfectpixel {
	namespace physics {

		void ColliderComponent::SetMaskRectangle(ecs::Entity entity, const bedrock::AARectangle &rectangle)
		{
			ColliderComponent::MaskType(entity) = RECTANGLE;
			ColliderComponent::Mask(entity).m_rectangle = rectangle;
		}

		bedrock::AARectangle ColliderComponent::SetMaskRectangle(ecs::Entity entity)
		{
			if (ColliderComponent::MaskType(entity) != RECTANGLE)
			{
				bedrock::PpException("Invalid mask type");
			}
			return ColliderComponent::Mask(entity).m_rectangle;
		}

		void ColliderComponent::SetMaskCircle(ecs::Entity entity, const bedrock::Circle &circle)
		{
			ColliderComponent::MaskType(entity) = CIRCLE;
			ColliderComponent::Mask(entity).m_circle = circle;
		}

		const perfectpixel::bedrock::Circle ColliderComponent::GetMaskCircle(ecs::Entity entity)
		{
			if (ColliderComponent::MaskType(entity) != CIRCLE)
			{
				bedrock::PpException("Invalid mask type");
			}
			return ColliderComponent::Mask(entity).m_circle;
		}

		void ColliderComponent::GetNear(std::vector<ecs::Entity> &toCheck, const bedrock::Vector2 &point)
		{
			bedrock::BitSet mask = ecs::EntityManager::getInstance()->all();
			Filter(mask, IComponentStorage::WITH);
			ecs::EntityManager::getInstance()->expandMask(mask, &toCheck, nullptr);
		}

	}
}

perfectpixel::serialization::ISerializer & operator<<(perfectpixel::serialization::ISerializer &ostream, const perfectpixel::physics::ColliderComponent::ColliderMask &mask)
{
	ostream.writeBinary(&mask, sizeof(perfectpixel::physics::ColliderComponent::ColliderMask));
	return ostream;
}

perfectpixel::serialization::ISerializer & operator>>(perfectpixel::serialization::ISerializer &istream, perfectpixel::physics::ColliderComponent::ColliderMask &mask)
{
	istream.readBinary(&mask, sizeof(perfectpixel::physics::ColliderComponent::ColliderMask));
	return istream;
}
