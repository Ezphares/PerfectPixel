#include <physics/ColliderComponent.h>

#include <types/PpException.h>

namespace perfectpixel {
	namespace physics {

		ColliderComponent::ColliderComponent(world::Entity entity)
			: m_entity(entity)
			, m_type(UNSET)
		{
		}


		ColliderComponent::ColliderComponent(world::Entity entity, const types::AARectangle &rectangle)
			: m_entity(entity)
		{
			setMaskRectangle(rectangle);
		}

		ColliderComponent::ColliderComponent(world::Entity entity, const types::Circle &circle)
			: m_entity(entity)
		{
			setMaskCircle(circle);
		}

		ColliderComponent::~ColliderComponent()
		{
		}

		perfectpixel::world::Entity ColliderComponent::getEntity() const
		{
			return m_entity;
		}

		void ColliderComponent::setMaskRectangle(const types::AARectangle &rectangle)
		{
			m_type = RECTANGLE;
			m_rectangle = rectangle;
		}

		types::AARectangle ColliderComponent::getMaskRectangle() const
		{
			if (m_type != RECTANGLE)
			{
				types::PpException("Invalid mask type");
			}
			return m_rectangle;
		}

		void ColliderComponent::setMaskCircle(const types::Circle &circle)
		{
			m_type = CIRCLE;
			m_circle = circle;
		}

		const perfectpixel::types::Circle ColliderComponent::getMaskCircle() const
		{
			if (m_type != CIRCLE)
			{
				types::PpException("Invalid mask type");
			}
			return m_circle;
		}

		ColliderComponent::MaskType ColliderComponent::getMaskType() const
		{
			return m_type;
		}

	}
}
