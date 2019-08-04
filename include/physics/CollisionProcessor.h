#pragma once

#include <physics/ColliderComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/TransformComponent.h>

#include <EntityComponentSystem/Entity.h>

#include <set>
#include <vector>

namespace perfectpixel { namespace physics {

	class CollisionProcessor : public ecs::Processor
	{
	private:
		struct CollisionData {
			inline CollisionData()
			{};
			ecs::Entity m_first, m_second;
			ColliderComponent::ColliderMaskType m_maskTypeFirst, m_maskTypeSecond;
			union {
				bedrock::Vector2 m_data_RectRectOverlap;
				float m_data_CircCircOverlap;
			};
		};

	public:
		CollisionProcessor();
		virtual ~CollisionProcessor();

		virtual void onUpdate(const std::vector<ecs::Entity> &entities, float deltaT) override;

		// Resolution utilities
		void singleAxisReposition(float mass1, float mass2, float overlap, float *out_magnitude1, float *out_magnitude2);
		void singleAxisBounce(
			float bounciness,
			float mass1,
			float mass2,
			float velocity1,
			float velocity2,
			float *out_newVelocity1,
			float *out_newVelocity2);

		uint32_t getCollisionsLastUpdate();

	private:
		void collideSingle(ecs::Entity entity, std::set<ecs::Entity> &cache);
		void possibleCollisions(const ecs::Entity entity, std::set<ecs::Entity> &cache, std::vector<ecs::Entity> &out_possibleCollisions);

		bool checkCollision(ecs::Entity first, ecs::Entity second, CollisionData &out_collision);
		bool collideRectRect(ecs::Entity first, const bedrock::AARectangle &firstRect, ecs::Entity second, const bedrock::AARectangle &secondRect, CollisionData &out_collision);
		bool collideCircleCircle(ecs::Entity first, const bedrock::Circle &firstCircle, ecs::Entity second, const bedrock::Circle &secondCircle, CollisionData &out_collision);

		void resolveCollision(const CollisionData &collision);

		bedrock::Vector2 absoluteCenter(ecs::Entity entity);

	private:
		uint32_t m_collisionCount;
	};

} }
