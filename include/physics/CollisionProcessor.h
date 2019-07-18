#pragma once

#include <physics/ColliderComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <EntityComponentSystem/Entity.h>

#include <set>
#include <vector>

namespace tests {
    class test_CollisionProcessor;
}

namespace perfectpixel { namespace physics {

	class CollisionProcessor : public ecs::Processor
	{
		friend class tests::test_CollisionProcessor;
	private:
		struct CollisionData {
			inline CollisionData()
			{};
			ecs::Entity m_first, m_second;
			ColliderComponent::ColliderMaskType m_maskTypeFirst, m_maskTypeSecond;
			union {
				bedrock::Vector2 m_data_RectRectOverlap;
				bedrock::PpFloat m_data_CircCircOverlap;
			};
		};

	public:
		CollisionProcessor();
		virtual ~CollisionProcessor();

		virtual void onUpdate(const std::vector<ecs::Entity> &entities, bedrock::PpFloat deltaT);

	private:
		void collideSingle(ecs::Entity entity, std::set<ecs::Entity> &cache);
		void possibleCollisions(const ecs::Entity entity, std::set<ecs::Entity> &cache, std::vector<ecs::Entity> &out_possibleCollisions);

		bool checkCollision(ecs::Entity first, ecs::Entity second, CollisionData &out_collision);
		bool collideRectRect(ecs::Entity first, const bedrock::AARectangle &firstRect, ecs::Entity second, const bedrock::AARectangle &secondRect, CollisionData &out_collision);
		bool collideCircleCircle(ecs::Entity first, const bedrock::Circle &firstCircle, ecs::Entity second, const bedrock::Circle &secondCircle, CollisionData &out_collision);

		void resolveCollision(const CollisionData &collision);
		void singleAxisReposition(bedrock::PpFloat mass1, bedrock::PpFloat mass2, bedrock::PpFloat overlap, bedrock::PpFloat *out_magnitude1, bedrock::PpFloat *out_magnitude2);
		void singleAxisBounce(
			bedrock::PpFloat bounciness,
			bedrock::PpFloat mass1,
			bedrock::PpFloat mass2,
			bedrock::PpFloat velocity1,
			bedrock::PpFloat velocity2,
			bedrock::PpFloat *out_newVelocity1,
			bedrock::PpFloat *out_newVelocity2);

		bedrock::Vector2 absoluteCenter(ecs::Entity entity);
	};

} }
