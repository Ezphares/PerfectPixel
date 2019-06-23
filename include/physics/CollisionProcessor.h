#pragma once

#include <physics/ColliderComponent.h>

#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/Entity.h>
#include <EntityComponentSystem/TransformComponent.h>

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
				types::Vector2 m_data_RectRectOverlap;
				types::PpFloat m_data_CircCircOverlap;
			};
		};

	public:
		CollisionProcessor(
			ecs::EntityManager *entityManager);
		virtual ~CollisionProcessor();

		virtual void onProcess(const std::vector<ecs::Entity> &entities, types::PpFloat deltaT);

	private:
		void collideSingle(ecs::Entity entity, std::set<ecs::Entity> &cache);
		void possibleCollisions(const ecs::Entity entity, std::set<ecs::Entity> &cache, std::vector<ecs::Entity> &out_possibleCollisions);

		bool checkCollision(ecs::Entity first, ecs::Entity second, CollisionData &out_collision);
		bool collideRectRect(ecs::Entity first, const types::AARectangle &firstRect, ecs::Entity second, const types::AARectangle &secondRect, CollisionData &out_collision);

		void resolveCollision(const CollisionData &collision);
		void singleAxisReposition(types::PpFloat mass1, types::PpFloat mass2, types::PpFloat overlap, types::PpFloat *out_magnitude1, types::PpFloat *out_magnitude2);
		void singleAxisBounce(
			types::PpFloat bounciness,
			types::PpFloat mass1,
			types::PpFloat mass2,
			types::PpFloat velocity1,
			types::PpFloat velocity2,
			types::PpFloat *out_newVelocity1,
			types::PpFloat *out_newVelocity2);

		types::Vector2 absoluteCenter(ecs::Entity entity);

	private:
		ecs::EntityManager *m_entityManager;
	};

} }
