#pragma once

#include <types/AARectangle.h>
#include <types/Circle.h>

#include <EntityComponentSystem/Entity.h>
#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <string>

namespace perfectpixel { 
namespace ecs {
	class EntityManager;
}
namespace physics {

	class ColliderComponent 
		: public ecs::Component<ColliderComponent>
		, public ecs::LinearScanComponentStorage
	{
	public:
		enum ColliderMaskType {
			UNSET,
			RECTANGLE,
			CIRCLE
		};

		union ColliderMask {
			ColliderMask(){}
			~ColliderMask() {}

			types::AARectangle m_rectangle;
			types::Circle m_circle;
		};

	public:
		static void SetMaskRectangle(ecs::Entity entity, const types::AARectangle &rectangle);
		static types::AARectangle SetMaskRectangle(ecs::Entity entity);

		static void SetMaskCircle(ecs::Entity entity, const types::Circle &circle);
		static const types::Circle GetMaskCircle(ecs::Entity entity);

		static void GetNear(std::vector<ecs::Entity> &toCheck, const types::Vector2 &point);

	public:
		_Field(ColliderComponent, ColliderMaskType, MaskType);
		_Field(ColliderComponent, ColliderMask, Mask);
		_Field(ColliderComponent, std::string, EventTag);
	};

} }
