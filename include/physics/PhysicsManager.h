#pragma once

#include <physics/TransformComponent.h>
#include <physics/PhysicsComponent.h>
#include <physics/ColliderComponent.h>
#include <physics/Force.h>

#include <worldgraph/EntityManager.h>
#include <types/numbers.h>
#include <types/Rectangle.h>

#include <map>
#include <set>
#include <vector>

namespace tests { class test_PhysicsManager; }

namespace perfectpixel
{
namespace physics
{
	class PhysicsManager
	{
		friend class tests::test_PhysicsManager;

	private:
		struct CollisionData {
			inline CollisionData() {};
			const ColliderComponent *m_firstCollider, *m_secondCollider;
			ColliderComponent::MaskType m_maskTypeFirst, m_maskTypeSecond;
			union {
				types::Vector2 m_data_RectRectOverlap;
			};
		};

	public:
		PhysicsManager(world::EntityManager *entityManager);
		~PhysicsManager();

	public:
		// Lifecycle functions
		void update(types::PpFloat deltaTime);
		void cleanup();

		// Component functions
		TransformComponent &getTransform(world::Entity entity);
		types::Vector3 getPosition(world::Entity entity);
		
		void registerPhysics(const PhysicsComponent &physicsComponent);
		bool hasPhysics(world::Entity entity) const;
		PhysicsComponent &getPhysics(world::Entity entity);

		void registerCollider(const ColliderComponent &collider);
		bool hasCollider(world::Entity entity);
		ColliderComponent &getCollider(world::Entity entity);

		// Force functions
		void pulseForce(world::Entity entity, const Force &force, types::PpFloat deltaTime);
		void setConstantForce(world::Entity entity, const Force &force);
		void removeConstantForce(world::Entity entity, types::PpInt forceId);

		// Utility functions
		types::Vector2 absoluteCenter(const ColliderComponent &collider);

		void translate(world::Entity entiy, types::Vector3 vec);

		// Collision functions
	private:
		void handleCollisions();
		void handleCollisionSingle(const ColliderComponent &collider, std::set<world::Entity> &collisionCache);
		void possibleCollissions(const ColliderComponent &collider, std::set<world::Entity> &collisionCache, std::vector<ColliderComponent> *out_colliders);

		bool checkCollision(const ColliderComponent &first, const ColliderComponent &second, CollisionData *out_collision);
		bool collideRectRect(
			world::Entity first,
			const types::Rectangle &firstRect,
			world::Entity second,
			const types::Rectangle &secondRect,
			CollisionData *out_collision);

		void resolveCollision(const CollisionData &collision);
		void singleAxisReposition(types::PpFloat mass1, types::PpFloat mass2, types::PpFloat overlap, types::PpFloat *out_magnitude1, types::PpFloat *out_magnitude2);


	private:
		typedef std::map<world::Entity, TransformComponent> TransformMap;
		typedef std::map<world::Entity, PhysicsComponent> PhysicsMap;
		typedef std::map<types::PpInt, Force> ConstantForceList;
		typedef std::map<world::Entity, ConstantForceList> ConstantForceMap;
		typedef std::map<world::Entity, ColliderComponent> ColliderMap;

		world::EntityManager *m_entityManager;

		TransformMap m_transforms;
		PhysicsMap m_physics;
		ConstantForceMap m_constantForces;
		ColliderMap m_colliders;

		std::vector<world::Entity> m_cleanup;
	};
}
}




