#include <physics/CollisionProcessor.h>
#include <physics/ColliderComponent.h>
#include <physics/PhysicsComponent.h>

#include <EntityComponentSystem/TransformComponent.h>

#include <Bedrock/Logger.h>

#include <set>
#include <cmath>

namespace perfectpixel { namespace physics {

	namespace
	{
		float COLLISON_LEEWAY = 0.01f;
		typedef ecs::QueryHelper<
			ecs::With<ColliderComponent, ecs::TransformComponent>>
			CollisionQuery;
	}

	CollisionProcessor::CollisionProcessor()
		: Processor(CollisionQuery::build())
		, m_collisionCount(0u)
	{

	}

	CollisionProcessor::~CollisionProcessor()
	{

	}

	void CollisionProcessor::onUpdate(const std::vector<ecs::Entity> &entities, float deltaT)
	{
		(void)deltaT;

		m_collisionCount = 0u;
		std::set<ecs::Entity> collisionCache;

		for (ecs::Entity entity : entities)
		{
			collisionCache.insert(entity);
			collideSingle(entity, collisionCache);
		}
	}

	uint32_t CollisionProcessor::getCollisionsLastUpdate()
	{
		return m_collisionCount;
	}

	void CollisionProcessor::collideSingle(ecs::Entity entity, std::set<ecs::Entity> &cache)
	{
		std::vector<ecs::Entity> toCheck;
		possibleCollisions(entity, cache, toCheck);

		// Store mass as we might change it for resolving chained collisions
		float storeMass = PhysicsComponent::Has(entity) ? PhysicsComponent::Mass(entity) : 0.0f;

		for (ecs::Entity other : toCheck)
		{
			CollisionData collision;
			if (checkCollision(entity, other, collision))
			{
				++m_collisionCount;
				resolveCollision(collision);
			}
		}

		if (PhysicsComponent::Has(entity))
		{
			PhysicsComponent::Mass(entity) = storeMass;
		}
	}

	void CollisionProcessor::possibleCollisions(const ecs::Entity entity, std::set<ecs::Entity> &cache, std::vector<ecs::Entity> &out_possibleCollisions)
	{
		std::vector<ecs::Entity> toCheck;
		ColliderComponent::GetNear(toCheck, absoluteCenter(entity));

		for (auto other : toCheck)
		{
			if (cache.find(other) == cache.end())
			{
				out_possibleCollisions.push_back(other);
			}
		}
	}

	bool CollisionProcessor::checkCollision(ecs::Entity first, ecs::Entity second, CollisionData &out_collision)
	{
		out_collision.m_first = first;
		out_collision.m_second = second;


		if (ColliderComponent::MaskType(first) == ColliderComponent::RECTANGLE && ColliderComponent::MaskType(second) == ColliderComponent::RECTANGLE)
		{
			return collideRectRect(
				first, ColliderComponent::GetMaskRectangle(first),
				second, ColliderComponent::GetMaskRectangle(second),
				out_collision);
		}
		else if (ColliderComponent::MaskType(first) == ColliderComponent::CIRCLE && ColliderComponent::MaskType(second) == ColliderComponent::CIRCLE)
		{
			return collideCircleCircle(
				first, ColliderComponent::GetMaskCircle(first),
				second, ColliderComponent::GetMaskCircle(second),
				out_collision);
		}

		// TODO Call other mask types
		return false;
	}

	bool CollisionProcessor::collideRectRect(ecs::Entity first, const bedrock::AARectangle &firstRect, ecs::Entity second, const bedrock::AARectangle &secondRect, CollisionData &out_collision)
	{
		bedrock::Vector2
			firstPosition = bedrock::Vector2(ecs::TransformComponent::Position(first)),
			secondPosition = bedrock::Vector2(ecs::TransformComponent::Position(second));


		bedrock::Vector2 offset = (secondPosition + secondRect.m_center) - (firstPosition + firstRect.m_center);

		bedrock::Vector2 overlap{
			firstRect.m_halfSize.x() + secondRect.m_halfSize.x() - std::abs(offset.x()),
			firstRect.m_halfSize.y() + secondRect.m_halfSize.y() - std::abs(offset.y())
		};

		if (overlap.x() < COLLISON_LEEWAY || overlap.y() < COLLISON_LEEWAY)
		{
			// No collision
			return false;
		}

		out_collision.m_data_RectRectOverlap = overlap;
		out_collision.m_maskTypeFirst = ColliderComponent::ColliderMaskType::RECTANGLE;
		out_collision.m_maskTypeSecond = ColliderComponent::ColliderMaskType::RECTANGLE;

		return true;

	}

	bool CollisionProcessor::collideCircleCircle(ecs::Entity first, const bedrock::Circle &firstCircle, ecs::Entity second, const bedrock::Circle &secondCircle, CollisionData &out_collision)
	{
		bedrock::Vector2
			firstPosition = bedrock::Vector2(ecs::TransformComponent::Position(first)),
			secondPosition = bedrock::Vector2(ecs::TransformComponent::Position(second));

		bedrock::Vector2 offset = (secondPosition + secondCircle.m_center) - (firstPosition + firstCircle.m_center);
		float squareDistance = bedrock::Vector2::dot(offset, offset);
		float sumRadii = firstCircle.m_radius + secondCircle.m_radius;


		if (squareDistance > (sumRadii - COLLISON_LEEWAY) * (sumRadii - COLLISON_LEEWAY))
		{
			// No collision
			return false;
		}

		out_collision.m_data_CircCircOverlap = sumRadii - std::sqrt(squareDistance);
		out_collision.m_maskTypeFirst = ColliderComponent::ColliderMaskType::CIRCLE;
		out_collision.m_maskTypeSecond = ColliderComponent::ColliderMaskType::CIRCLE;

		return true;
	}

	void CollisionProcessor::resolveCollision(const CollisionData &collision)
	{
		// FIXME: Generate events

		if (!PhysicsComponent::Has(collision.m_first) || !PhysicsComponent::Has(collision.m_second))
		{
			// Without physics, resolution is done
			return;
		}

		bedrock::Vector2 
			resolution1{ 0,0 }, 
			resolution2{ 0,0 };
		bedrock::Vector2 
			bounce1{ ecs::TransformComponent::Velocity(collision.m_first) },
			bounce2{ ecs::TransformComponent::Velocity(collision.m_second) };

		float bounciness = std::max(PhysicsComponent::Bounciness(collision.m_first), PhysicsComponent::Bounciness(collision.m_second));

		if (collision.m_maskTypeFirst == ColliderComponent::ColliderMaskType::RECTANGLE &&
			collision.m_maskTypeSecond == ColliderComponent::ColliderMaskType::RECTANGLE)
		{
			bedrock::Vector2 overlap = collision.m_data_RectRectOverlap;
			float newVel1, newVel2;

			if (overlap.x() < overlap.y())
			{
				singleAxisReposition(PhysicsComponent::Mass(collision.m_first), PhysicsComponent::Mass(collision.m_second), overlap.x(), &(resolution1.m_data[0]), &(resolution2.m_data[0]));
				
				if (absoluteCenter(collision.m_first).x() > absoluteCenter(collision.m_second).x())
				{
					resolution2 *= -1;
				}
				else
				{
					resolution1 *= -1;
				}

				singleAxisBounce(bounciness, 
					PhysicsComponent::Mass(collision.m_first), PhysicsComponent::Mass(collision.m_second),
					ecs::TransformComponent::Velocity(collision.m_first).x(), ecs::TransformComponent::Velocity(collision.m_second).x(),
					&newVel1, &newVel2);

				bounce1 = { newVel1, ecs::TransformComponent::Velocity(collision.m_first).y() };
				bounce2 = { newVel2, ecs::TransformComponent::Velocity(collision.m_first).y() };
			}
			else
			{
				singleAxisReposition(PhysicsComponent::Mass(collision.m_first), PhysicsComponent::Mass(collision.m_second), overlap.y(), &(resolution1.m_data[1]), &(resolution2.m_data[1]));
				if (absoluteCenter(collision.m_first).y() > absoluteCenter(collision.m_second).y())
				{
					resolution2 *= -1;
				}
				else
				{
					resolution1 *= -1;
				}

				singleAxisBounce(bounciness, 
					PhysicsComponent::Mass(collision.m_first), PhysicsComponent::Mass(collision.m_second), 
					ecs::TransformComponent::Velocity(collision.m_first).y(), ecs::TransformComponent::Velocity(collision.m_second).y(),
					&newVel1, &newVel2);

				bounce1 = { ecs::TransformComponent::Velocity(collision.m_first).x(), newVel1 };
				bounce2 = { ecs::TransformComponent::Velocity(collision.m_second).x(), newVel2 };
			}
		}
		else if (
			collision.m_maskTypeFirst == ColliderComponent::ColliderMaskType::CIRCLE &&
			collision.m_maskTypeSecond == ColliderComponent::ColliderMaskType::CIRCLE)
		{
			bedrock::Vector2 resolutionAxis = (absoluteCenter(collision.m_second) - absoluteCenter(collision.m_first)).normal();

			float
				magnitude1,
				magnitude2;

			singleAxisReposition(PhysicsComponent::Mass(collision.m_first), PhysicsComponent::Mass(collision.m_second), collision.m_data_CircCircOverlap, &magnitude1, &magnitude2);

			resolution1 = resolutionAxis * -magnitude1;
			resolution2 = resolutionAxis * magnitude2;

			// FIXME bounce
		}
		else
		{
			// Unsupported collision type
		}

		ecs::TransformComponent::Position(collision.m_first) += bedrock::Vector3(resolution1);
		ecs::TransformComponent::Position(collision.m_second) += bedrock::Vector3(resolution2);

		ecs::TransformComponent::Velocity(collision.m_first) = bedrock::Vector3(bounce1);
		ecs::TransformComponent::Velocity(collision.m_second) = bedrock::Vector3(bounce2);
	}

	void CollisionProcessor::singleAxisReposition(float mass1, float mass2, float overlap, float *out_magnitude1, float *out_magnitude2)
	{
		*out_magnitude1 = 0;
		*out_magnitude2 = 0;

		// Special cases:
		if ((mass1 == 0 && mass2 == 0) || // Zero mass can never affect others
			(mass1 == bedrock::Infinity && mass2 == bedrock::Infinity)) // Infinite mass can never be affected
		{
			return;
		}

		// Get rid of infinities for math, we can be sure only one of them is infinity given above sanity check
		if (mass1 == bedrock::Infinity)
		{
			mass1 = 1;
			mass2 = 0;
		}
		else if (mass2 == bedrock::Infinity)
		{
			mass1 = 0;
			mass2 = 1;
		}

		*out_magnitude1 = mass2 / (mass1 + mass2) * overlap;
		*out_magnitude2 = mass1 / (mass1 + mass2) * overlap;
	}

	void CollisionProcessor::singleAxisBounce(float bounciness, float mass1, float mass2, float velocity1, float velocity2, float *out_newVelocity1, float *out_newVelocity2)
	{
		*out_newVelocity1 = velocity1;
		*out_newVelocity2 = velocity2;

		// Special cases:
		if ((mass1 == 0 && mass2 == 0) || // Zero mass can never affect others
			(mass1 == bedrock::Infinity && mass2 == bedrock::Infinity)) // Infinite mass can never be affected
		{
			return;
		}

		// Get rid of infinities for math, we can be sure only one of them is infinity given above sanity check
		if (mass1 == bedrock::Infinity)
		{
			mass1 = 1;
			mass2 = 0;
		}
		else if (mass2 == bedrock::Infinity)
		{
			mass1 = 0;
			mass2 = 1;
		}

		*out_newVelocity1 = (bounciness * mass2 * (velocity2 - velocity1) + mass1 * velocity1 + mass2 * velocity2) / (mass1 + mass2);
		*out_newVelocity2 = (bounciness * mass1 * (velocity1 - velocity2) + mass1 * velocity1 + mass2 * velocity2) / (mass1 + mass2);

	}

	bedrock::Vector2 CollisionProcessor::absoluteCenter(ecs::Entity entity)
	{
		bedrock::Vector2 result = bedrock::Vector2(ecs::TransformComponent::Position(entity));

		switch (ColliderComponent::MaskType(entity))
		{
		case ColliderComponent::RECTANGLE:
			result += AARectangleColliderMask::Center(entity);
			break;

		case ColliderComponent::CIRCLE:
			result += CircleColliderMask::Center(entity);
			break;

		default:
			break;
		}

		return result;
	}

} }
