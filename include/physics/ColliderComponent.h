#pragma once

#include <types/Rectangle.h>

#include <worldgraph/Entity.h>

namespace perfectpixel {
	namespace physics {

		class ColliderComponent
		{
		public:
			enum MaskType {
				UNSET,
				RECTANGLE
			};

		public:
			ColliderComponent(world::Entity entity);
			ColliderComponent(world::Entity entity, const types::Rectangle &rectangle);
			~ColliderComponent();

		public:
			world::Entity getEntity() const;

			void setMaskRectangle(const types::Rectangle &rectangle);
			types::Rectangle getMaskRectangle() const;
			MaskType getMaskType() const;

		private:
			world::Entity m_entity;

			MaskType m_type;

			union {
				types::Rectangle m_rectangle;
			};
		};

	}
}
