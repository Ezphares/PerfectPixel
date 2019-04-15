#pragma once

#include <types/AARectangle.h>
#include <types/Circle.h>

#include <EntityComponentSystem/Entity.h>

#include <string>

namespace perfectpixel {
	namespace physics {

		class ColliderComponent
		{
		public:
			enum MaskType {
				UNSET,
				RECTANGLE,
				CIRCLE
			};

		public:
			ColliderComponent(world::Entity entity);
			ColliderComponent(world::Entity entity, const types::AARectangle &rectangle);
			ColliderComponent(world::Entity entity, const types::Circle &circle);
			~ColliderComponent();

		public:
			world::Entity getEntity() const;
			MaskType getMaskType() const;

			void setMaskRectangle(const types::AARectangle &rectangle);
			types::AARectangle getMaskRectangle() const;

			void setMaskCircle(const types::Circle &circle);
			const types::Circle getMaskCircle() const;

			void setEventTag(const std::string &text);
			std::string getEventTag() const;

		private:
			world::Entity m_entity;

			MaskType m_type;

			union {
				types::AARectangle m_rectangle;
				types::Circle m_circle;
			};

			std::string m_eventTag;
		};

	}
}
