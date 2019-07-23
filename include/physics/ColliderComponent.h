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

		union ColliderMask {
			ColliderMask(){}
			~ColliderMask() {}

			bedrock::AARectangle m_rectangle;
			bedrock::Circle m_circle;
		};

	public:
		static void SetMaskRectangle(ecs::Entity entity, const bedrock::AARectangle &rectangle);
		static bedrock::AARectangle SetMaskRectangle(ecs::Entity entity);

		static void SetMaskCircle(ecs::Entity entity, const bedrock::Circle &circle);
		static const bedrock::Circle GetMaskCircle(ecs::Entity entity);

		static void GetNear(std::vector<ecs::Entity> &toCheck, const bedrock::Vector2 &point);

	public:
		PPField(ColliderComponent, ColliderMaskType, MaskType);
		PPField(ColliderComponent, ColliderMask, Mask);
		PPField(ColliderComponent, std::string, EventTag);
	};

} }

perfectpixel::serialization::ISerializer &operator<<(perfectpixel::serialization::ISerializer &ostream, const perfectpixel::physics::ColliderComponent::ColliderMask &mask);
perfectpixel::serialization::ISerializer &operator>>(perfectpixel::serialization::ISerializer &istream, perfectpixel::physics::ColliderComponent::ColliderMask &mask);
