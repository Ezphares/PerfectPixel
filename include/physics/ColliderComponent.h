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
		static void setMaskRectangle(ecs::Entity entity, const types::AARectangle &rectangle);
		static types::AARectangle getMaskRectangle(ecs::Entity entity);

		static void setMaskCircle(ecs::Entity entity, const types::Circle &circle);
		static const types::Circle getMaskCircle(ecs::Entity entity);

		static void GetNear(ecs::EntityManager *entityManager, std::vector<ecs::Entity> &toCheck, const types::Vector2 &point);

	public:
		inline static ecs::Field<ColliderComponent, ColliderMaskType> MaskType;
		inline static ecs::Field<ColliderComponent, ColliderMask> Mask;
		inline static ecs::Field<ColliderComponent, std::string> EventTag;
	};

} }
