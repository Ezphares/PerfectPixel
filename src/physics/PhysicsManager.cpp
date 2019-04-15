#include <physics/PhysicsManager.h>

#include <types/PpException.h>

#include <functional>

#include <cmath>

namespace perfectpixel
{
	namespace physics
	{
		namespace
		{
			types::PpFloat COLLISON_LEEWAY = 0.01f;
		}
		
		PhysicsManager::PhysicsManager(ecs::EntityManager *entityManager)
			: m_entityManager(entityManager)
		{
		}


		PhysicsManager::~PhysicsManager()
		{
		}

		void PhysicsManager::update(types::PpFloat deltaTime)
		{
			for (auto &it : m_transforms)
			{

				if (!m_entityManager->isAlive(it.first))
				{
					m_cleanup.push_back(it.first);
					continue;
				}
				

				// Apply constant forces such as gravity
				auto forceList = m_constantForces.find(it.first);
				if (forceList != m_constantForces.end())
				{
					for (auto idForcePair : forceList->second)
					{
						pulseForce(it.first, idForcePair.second, deltaTime);
					}
				}

				// Update positions
				TransformComponent &transform = it.second;
				transform.m_position += (transform.m_velocity * deltaTime);

				// Run collisions
				handleCollisions();
			}
		}

		void PhysicsManager::cleanup()
		{
			for (auto &entity : m_cleanup)
			{
				// Remove any physics related components from the entity
				m_transforms.erase(entity);
				m_physics.erase(entity);
				m_constantForces.erase(entity);
				m_colliders.erase(entity);
			}
			m_cleanup.clear();
		}

		perfectpixel::physics::TransformComponent &PhysicsManager::getTransform(ecs::Entity entity)
		{
			auto it = m_transforms.find(entity);
			if (it == m_transforms.end())
			{
				// Emplace magic
				it = m_transforms.emplace(std::make_pair(entity, entity)).first;
			}
			return it->second;
		}

		perfectpixel::types::Vector3 PhysicsManager::getPosition(ecs::Entity entity)
		{
			return getTransform(entity).m_position;
		}

		void PhysicsManager::registerPhysics(const PhysicsComponent &physicsComponent)
		{
			m_physics.emplace(physicsComponent.getEntity(), physicsComponent);
			// The physics component does not has its own update cycle so we have to ensure that it has
			// a transform component, otherwise PhysicsComponents could get leaked (albeit at a small chance)
			getTransform(physicsComponent.getEntity());
		}

		bool PhysicsManager::hasPhysics(ecs::Entity entity) const
		{
			return m_physics.find(entity) != m_physics.end();
		}

		perfectpixel::physics::PhysicsComponent & PhysicsManager::getPhysics(ecs::Entity entity)
		{
			auto it = m_physics.find(entity);
			if (it == m_physics.end())
			{
				throw types::PpException("Tried to get physics component for entity with none assigned");
			}
			return it->second;
		}

		void PhysicsManager::registerCollider(const ColliderComponent &collider)
		{
			m_colliders.emplace(collider.getEntity(), collider);
		}

		bool PhysicsManager::hasCollider(ecs::Entity entity)
		{
			return m_colliders.find(entity) != m_colliders.end();
		}

		perfectpixel::physics::ColliderComponent & PhysicsManager::getCollider(ecs::Entity entity)
		{
			auto it = m_colliders.find(entity);
			if (it == m_colliders.end())
			{
				throw types::PpException("Tried to get collider component for entity with none assigned");
			}

			return it->second;
		}

		void PhysicsManager::pulseForce(ecs::Entity entity, const Force &force, types::PpFloat deltaTime)
		{
			// TODO Alive Check 
			
			TransformComponent &transform = getTransform(entity);

			types::Vector3 resultForce = force.m_vector * (force.m_relativeMagnitude ? transform.m_velocity.magnitude() : 1);

			if (force.m_relativeDirection)
			{
				


				// TODO: Implement this
			}

			transform.m_velocity += resultForce * deltaTime;
		}

		void PhysicsManager::setConstantForce(ecs::Entity entity, const Force &force)
		{
			m_constantForces[entity][force.m_identifier] = force;
		}

		void PhysicsManager::removeConstantForce(ecs::Entity entity, types::PpInt forceId)
		{
			auto forces = m_constantForces.find(entity);
			if (forces == m_constantForces.end())
			{
				return;
			}
			forces->second.erase(forceId);
		}

		perfectpixel::types::Vector2 PhysicsManager::absoluteCenter(const ColliderComponent &collider)
		{
			types::Vector2 result = types::Vector2(getTransform(collider.getEntity()).m_position);
			switch (collider.getMaskType())
			{
			case ColliderComponent::RECTANGLE:
				result += collider.getMaskRectangle().m_center;
				break;

			case ColliderComponent::CIRCLE:
				result += collider.getMaskCircle().m_center;
				break;

			default:
				break;
			}

			return result;
		}

		perfectpixel::ecs::PositionCallback PhysicsManager::positionCallback()
		{
			return std::bind(&physics::PhysicsManager::getPosition, this, std::placeholders::_1);
		}

		void PhysicsManager::translate(ecs::Entity entiy, types::Vector3 vec)
		{
			getTransform(entiy).m_position += vec;
		}

		bool PhysicsManager::checkCollision(const ColliderComponent &first, const ColliderComponent &second, CollisionData *out_collision)
		{
			out_collision->m_firstCollider = &first;
			out_collision->m_secondCollider = &second;

			if (first.getMaskType() == ColliderComponent::RECTANGLE && second.getMaskType() == ColliderComponent::RECTANGLE)
			{
				return collideRectRect(first.getEntity(), first.getMaskRectangle(), second.getEntity(), second.getMaskRectangle(), out_collision);
			}

			// TODO Call other mask types
			return false;
		}

		bool PhysicsManager::collideRectRect(ecs::Entity first, const types::AARectangle &firstRect, ecs::Entity second, const types::AARectangle &secondRect, CollisionData *out_collision)
		{
			TransformComponent
				firstTransform = getTransform(first),
				secondTransform = getTransform(second);

			types::Vector2 offset{
				(secondTransform.m_position.x() + secondRect.m_center.x()) - (firstTransform.m_position.x() + firstRect.m_center.x()),
				(secondTransform.m_position.y() + secondRect.m_center.y()) - (firstTransform.m_position.y() + firstRect.m_center.y())
			};

			types::Vector2 overlap {
				firstRect.m_halfSize.x() + secondRect.m_halfSize.x() - std::abs(offset.x()),
				firstRect.m_halfSize.y() + secondRect.m_halfSize.y() - std::abs(offset.y())
			};

			if (overlap.x() < COLLISON_LEEWAY || overlap.y() < COLLISON_LEEWAY)
			{
				// No collision
				return false;
			}

			out_collision->m_data_RectRectOverlap = overlap;
			out_collision->m_maskTypeFirst = ColliderComponent::MaskType::RECTANGLE;
			out_collision->m_maskTypeSecond = ColliderComponent::MaskType::RECTANGLE;

			return true;

		}

		bool PhysicsManager::collideCircleCircle(ecs::Entity first, const types::Circle &firstCircle, ecs::Entity second, const types::Circle &secondCircle, CollisionData *out_collision)
		{
			TransformComponent
				firstTransform = getTransform(first),
				secondTransform = getTransform(second);

			types::Vector2 offset{
				(secondTransform.m_position.x() + secondCircle.m_center.x()) - (firstTransform.m_position.x() + firstCircle.m_center.x()),
				(secondTransform.m_position.y() + secondCircle.m_center.y()) - (firstTransform.m_position.y() + firstCircle.m_center.y())
			};

			types::PpFloat threshold = (firstCircle.m_radius + secondCircle.m_radius);
			types::PpFloat distance2 = types::Vector2::dot(offset, offset);

			if (threshold - distance2 > COLLISON_LEEWAY)
			{
				return false;
			}

			out_collision->m_data_CircCircOverlap = threshold - sqrtf(distance2);
			out_collision->m_maskTypeFirst = ColliderComponent::MaskType::CIRCLE;
			out_collision->m_maskTypeSecond = ColliderComponent::MaskType::CIRCLE;

			return true;
		}

		void PhysicsManager::resolveCollision(const CollisionData &collision)
		{
			// FIXME: Generate events

			ecs::Entity
				first = collision.m_firstCollider->getEntity(),
				second = collision.m_secondCollider->getEntity();

			if (!hasPhysics(first) || !hasPhysics(second))
			{
				// Without physics, resolution is done
				return;
			}

			PhysicsComponent
				&firstPhysics = getPhysics(first),
				&secondPhysics = getPhysics(second);

			TransformComponent
				&firstTransform = getTransform(first),
				&secondTransform = getTransform(second);

			types::Vector2 resolution1{ 0,0 }, resolution2{ 0,0 };
			types::Vector2 bounce1{ firstTransform.m_velocity }, bounce2{ secondTransform.m_velocity };

			types::PpFloat bounciness = std::max(firstPhysics.getBounciness(), secondPhysics.getBounciness());

			if (collision.m_maskTypeFirst == ColliderComponent::MaskType::RECTANGLE &&
				collision.m_maskTypeSecond == ColliderComponent::MaskType::RECTANGLE)
			{
				types::Vector2 overlap = collision.m_data_RectRectOverlap;
				types::PpFloat newVel1, newVel2;

				if (overlap.x() < overlap.y())
				{
					singleAxisReposition(firstPhysics.getMass(), secondPhysics.getMass(), overlap.x(), &(resolution1.m_data[0]), &(resolution2.m_data[0]));
					if (absoluteCenter(*collision.m_firstCollider).x() > absoluteCenter(*collision.m_secondCollider).x())
					{
						resolution2 *= -1;
					}
					else
					{
						resolution1 *= -1;
					}

					singleAxisBounce(bounciness, firstPhysics.getMass(), secondPhysics.getMass(), firstTransform.m_velocity.x(), secondTransform.m_velocity.x(), &newVel1, &newVel2);
					bounce1 = { newVel1, firstTransform.m_velocity.y() };
					bounce2 = { newVel2, secondTransform.m_velocity.y() };
				}
				else
				{
					singleAxisReposition(firstPhysics.getMass(), secondPhysics.getMass(), overlap.y(), &(resolution1.m_data[1]), &(resolution2.m_data[1]));
					if (absoluteCenter(*collision.m_firstCollider).y() > absoluteCenter(*collision.m_secondCollider).y())
					{
						resolution2 *= -1;
					}
					else
					{
						resolution1 *= -1;
					}

					singleAxisBounce(bounciness, firstPhysics.getMass(), secondPhysics.getMass(), firstTransform.m_velocity.y(), secondTransform.m_velocity.y(), &newVel1, &newVel2);
					bounce1 = { firstTransform.m_velocity.x(), newVel1 };
					bounce2 = { secondTransform.m_velocity.x(), newVel2 };
				}
			}
			else if (
				collision.m_maskTypeFirst == ColliderComponent::MaskType::CIRCLE &&
				collision.m_maskTypeSecond == ColliderComponent::MaskType::CIRCLE)
			{
				types::Vector2 resolutionAxis = (absoluteCenter(*collision.m_secondCollider) - absoluteCenter(*collision.m_firstCollider)).normal();

				types::PpFloat
					magnitude1,
					magnitude2;

				singleAxisReposition(firstPhysics.getMass(), secondPhysics.getMass(), collision.m_data_CircCircOverlap, &magnitude1, &magnitude2);
				
				resolution1 = resolutionAxis * -magnitude1;
				resolution2 = resolutionAxis * magnitude2;

				// FIXME bounce
			}
			else
			{
				// Unsupported collision type
			}

			translate(first, types::Vector3(resolution1));
			translate(second, types::Vector3(resolution2));

			firstTransform.m_velocity = types::Vector3(bounce1);
			secondTransform.m_velocity = types::Vector3(bounce2);
		}

		void PhysicsManager::singleAxisReposition(types::PpFloat mass1, types::PpFloat mass2, types::PpFloat overlap, types::PpFloat *out_magnitude1, types::PpFloat *out_magnitude2)
		{
			*out_magnitude1 = 0;
			*out_magnitude2 = 0;

			// Special cases:
			if ((mass1== 0 && mass2 == 0) || // Zero mass can never affect others
				(mass1 == types::Infinity && mass2 == types::Infinity)) // Infinite mass can never be affected
			{
				return;
			}

			// Get rid of infinities for math, we can be sure only one of them is infinity given above sanity check
			if (mass1 == types::Infinity)
			{
				mass1 = 1;
				mass2 = 0;
			}
			else if (mass2 == types::Infinity)
			{
				mass1 = 0;
				mass2 = 1;
			}

			*out_magnitude1 = mass2 / (mass1 + mass2) * overlap;
			*out_magnitude2 = mass1 / (mass1 + mass2) * overlap;
		}

		void PhysicsManager::singleAxisBounce(types::PpFloat bounciness, types::PpFloat mass1, types::PpFloat mass2, types::PpFloat velocity1, types::PpFloat velocity2, types::PpFloat *out_newVelocity1, types::PpFloat *out_newVelocity2)
		{
			*out_newVelocity1 = velocity1;
			*out_newVelocity2 = velocity2;

			// Special cases:
			if ((mass1 == 0 && mass2 == 0) || // Zero mass can never affect others
				(mass1 == types::Infinity && mass2 == types::Infinity)) // Infinite mass can never be affected
			{
				return;
			}

			// Get rid of infinities for math, we can be sure only one of them is infinity given above sanity check
			if (mass1 == types::Infinity)
			{
				mass1 = 1;
				mass2 = 0;
			}
			else if (mass2 == types::Infinity)
			{
				mass1 = 0;
				mass2 = 1;
			}

			*out_newVelocity1 = (bounciness * mass2 * (velocity2 - velocity1) + mass1 * velocity1 + mass2 * velocity2) / (mass1 + mass2);
			*out_newVelocity2 = (bounciness * mass1 * (velocity1 - velocity2) + mass1 * velocity1 + mass2 * velocity2) / (mass1 + mass2);
		}

		void PhysicsManager::handleCollisions()
		{
			for (auto it = m_colliders.begin(); it != m_colliders.end(); ++it)
			{
				std::set<ecs::Entity> collisionCache;
				handleCollisionSingle(it->second, collisionCache);
			}
		}

		void PhysicsManager::handleCollisionSingle(const ColliderComponent &collider, std::set<ecs::Entity> &collisionCache)
		{
			// Entity alive check
			if (!m_entityManager->isAlive(collider.getEntity()))
			{
				m_cleanup.push_back(collider.getEntity());
				return;
			}
			
			std::vector<ColliderComponent> toCheck;
			possibleCollissions(collider, collisionCache, &toCheck);

			// Store mass as we might change it for resolving chained collisions
			types::PpFloat storeMass = hasPhysics(collider.getEntity()) ? getPhysics(collider.getEntity()).getMass() : 0;

			for (auto &other : toCheck)
			{
				CollisionData collision;
				if (checkCollision(collider, other, &collision))
				{
					resolveCollision(collision);
				}
			}

			if (hasPhysics(collider.getEntity()))
			{
				getPhysics(collider.getEntity()).setMass(storeMass);
			}
		}

		void PhysicsManager::possibleCollissions(const ColliderComponent &collider, std::set<ecs::Entity> &collisionCache, std::vector<ColliderComponent> *out_colliders)
		{
			// FIXME store colliders in a QuadTree instead, so that we can partition
			for (auto it : m_colliders)
			{
				ecs::Entity other = it.second.getEntity();

				if (collider.getEntity() != other
					&& m_entityManager->isAlive(other)
					&& collisionCache.insert(other).second)
				{
					out_colliders->push_back(it.second);
				}
			}
		}

	}
}
