#pragma once

#include <Bedrock/AARectangle.h>
#include <Bedrock/Circle.h>

#include <EntityComponentSystem/Entity.h>
#include <EntityComponentSystem/Component.h>
#include <EntityComponentSystem/CoreComponentStorage.h>

#include <string>

namespace perfectpixel { 
namespace ecs {
	class EntityManager;
}
namespace serialization {
	class BinarySerializer;
}
namespace physics {

	class AARectangleColliderMask
		: public ecs::Component<AARectangleColliderMask>
		, public ecs::MapComponentStorage
	{
	public:
		PPField(AARectangleColliderMask, bedrock::Vector2, Center);
		PPField(AARectangleColliderMask, bedrock::Vector2, HalfSize);
	};

	class CircleColliderMask
		: public ecs::Component<CircleColliderMask>
		, public ecs::MapComponentStorage
	{
	public:
		PPField(CircleColliderMask, bedrock::Vector2, Center);
		PPField(CircleColliderMask, float, Radius);
	};

	class ColliderComponent 
		: public ecs::Component<ColliderComponent>
		, public ecs::LinearScanComponentStorage
	{
	public:
		enum ColliderMaskType : uint32_t {
			UNSET,
			RECTANGLE,
			CIRCLE
		};

	public:
		static void ClearMask(ecs::Entity entity);

		static void SetMaskRectangle(ecs::Entity entity, const bedrock::AARectangle &rectangle);
		static bedrock::AARectangle GetMaskRectangle(ecs::Entity entity);

		static void SetMaskCircle(ecs::Entity entity, const bedrock::Circle &circle);
		static const bedrock::Circle GetMaskCircle(ecs::Entity entity);

		static void GetNear(std::vector<ecs::Entity> &toCheck, const bedrock::Vector2 &point);

	public:
		PPField(ColliderComponent, ColliderMaskType, MaskType);
		PPField(ColliderComponent, std::string, EventTag);
	};

} }
