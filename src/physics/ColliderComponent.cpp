#include <physics/ColliderComponent.h>

namespace perfectpixel {
	namespace physics {

		ColliderComponent::ColliderComponent(world::Entity entity)
			: m_entity(entity)
			, m_type(UNSET)
		{
		}


		ColliderComponent::ColliderComponent(world::Entity entity, const types::Rectangle &rectangle)
			: m_entity(entity)
		{
			setMaskRectangle(rectangle);
		}

		ColliderComponent::~ColliderComponent()
		{
		}

		perfectpixel::world::Entity ColliderComponent::getEntity() const
		{
			return m_entity;
		}

		void ColliderComponent::setMaskRectangle(const types::Rectangle &rectangle)
		{
			m_type = RECTANGLE;
			m_rectangle = rectangle;
		}

		types::Rectangle ColliderComponent::getMaskRectangle() const
		{
			if (m_type != RECTANGLE)
			{
				throw "Invalid mask type";
			}
			return m_rectangle;
		}

		ColliderComponent::MaskType ColliderComponent::getMaskType() const
		{
			return m_type;
		}

	}
}
