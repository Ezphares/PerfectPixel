#include <physics/PhysicsManager.h>

#include <cmath>

namespace perfectpixel
{
	namespace physics
	{
		namespace
		{
			types::PpFloat COLLISON_LEEWAY = 0.01f;
		}
		
		PhysicsManager::PhysicsManager(world::EntityManager *entityManager)
			: m_entityManager(entityManager)
		{
		}


		PhysicsManager::~PhysicsManager()
		{
		}

		void PhysicsManager::update(types::PpFloat deltaTime)
		{
			for (auto it : m_transforms)
			{
				/* TODO:
				if (m_entityManager.isAlive(it.first))
				{
					m_cleanup.push_back(it.first);
					continue;
				}
				*/

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
				// TODO
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

		perfectpixel::physics::TransformComponent &PhysicsManager::getTransform(world::Entity entity)
		{
			auto it = m_transforms.find(entity);
			if (it == m_transforms.end())
			{
				// Emplace magic
				it = m_transforms.emplace(std::make_pair(entity, entity)).first;
			}
			return it->second;
		}

		perfectpixel::types::Vector3 PhysicsManager::getPosition(world::Entity entity)
		{
			return getTransform(entity).m_position;
		}

		bool PhysicsManager::hasPhysics(world::Entity entity) const
		{
			return m_physics.find(entity) != m_physics.end();
		}

		perfectpixel::physics::PhysicsComponent & PhysicsManager::getPhysics(world::Entity entity)
		{
			auto it = m_physics.find(entity);
			if (it == m_physics.end())
			{
				throw "Entity did not have PhysicsComponent"; // TODO Exception
			}
			return it->second;
		}

		void PhysicsManager::pulseForce(world::Entity entity, const Force &force, types::PpFloat deltaTime)
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

		void PhysicsManager::setConstantForce(world::Entity entity, const Force &force)
		{
			m_constantForces[entity][force.m_identifier] = force;
		}

		void PhysicsManager::removeConstantForce(world::Entity entity, types::PpInt forceId)
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

			default:
				break;
			}

			return result;
		}

		void PhysicsManager::translate(world::Entity entiy, types::Vector3 vec)
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

		bool PhysicsManager::collideRectRect(world::Entity first, const types::Rectangle &firstRect, world::Entity second, const types::Rectangle &secondRect, CollisionData *out_collision)
		{
			TransformComponent
				firstTransform = getTransform(first),
				secondTransform = getTransform(second);

			types::PpFloat
				xOffset = (secondTransform.m_position.m_x + secondRect.m_center.m_x) - (firstTransform.m_position.m_x + firstRect.m_center.m_x),
				yOffset = (secondTransform.m_position.m_y + secondRect.m_center.m_y) - (firstTransform.m_position.m_y + firstRect.m_center.m_y);

			types::Vector2 overlap {
				(firstRect.m_size.m_x + secondRect.m_size.m_x) / 2 - std::abs(xOffset),
				(firstRect.m_size.m_y + secondRect.m_size.m_y) / 2 - std::abs(yOffset)
			};

			if (overlap.m_x < COLLISON_LEEWAY || overlap.m_y < COLLISON_LEEWAY)
			{
				// No collision
				return false;
			}

			out_collision->m_data_RectRectOverlap = overlap;
			out_collision->m_maskTypeFirst = ColliderComponent::MaskType::RECTANGLE;
			out_collision->m_maskTypeSecond = ColliderComponent::MaskType::RECTANGLE;

			return true;

		}

		void PhysicsManager::resolveCollision(const CollisionData &collision)
		{
			// FIXME: Generate events

			world::Entity
				first = collision.m_firstCollider->getEntity(),
				second = collision.m_secondCollider->getEntity();

			if (!hasPhysics(first) || !hasPhysics(second))
			{
				// Without physics, resolution is done
				return;
			}

			types::Vector2 resolution1, resolution2;

			if (collision.m_maskTypeFirst == ColliderComponent::MaskType::RECTANGLE &&
				collision.m_maskTypeSecond == ColliderComponent::MaskType::RECTANGLE)
			{
				PhysicsComponent
					&firstPhysics = getPhysics(first),
					&secondPhysics = getPhysics(second);

				TransformComponent
					&firstTransform = getTransform(first),
					&secondTransform = getTransform(second);

				types::Vector2 overlap = collision.m_data_RectRectOverlap;
				
				// TODO: This currently works on entity transform coordinates, and can cause some wierd behaviour if the colliders have their center offset more than half their width
				if (overlap.m_x < overlap.m_y)
				{
					singleAxisReposition(firstPhysics.getMass(), secondPhysics.getMass(), overlap.m_x, &resolution1.m_x, &resolution2.m_x);
					if (absoluteCenter(*collision.m_firstCollider).m_x > absoluteCenter(*collision.m_secondCollider).m_x)
					{
						resolution2 *= -1;
					}
					else
					{
						resolution1 *= -1;
					}
				}
				else
				{
					singleAxisReposition(firstPhysics.getMass(), secondPhysics.getMass(), overlap.m_y, &resolution1.m_y, &resolution2.m_y);
					if (absoluteCenter(*collision.m_firstCollider).m_y > absoluteCenter(*collision.m_secondCollider).m_y)
					{
						resolution2 *= -1;
					}
					else
					{
						resolution1 *= -1;
					}
				}
			}

			translate(first, types::Vector3(resolution1));
			translate(second, types::Vector3(resolution2));
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

		void PhysicsManager::handleCollisions()
		{
			for (auto it = m_colliders.begin(); it != m_colliders.end(); ++it)
			{
				std::set<world::Entity> collisionCache;
				handleCollisionSingle(it->second, collisionCache);
			}
		}

		void PhysicsManager::handleCollisionSingle(const ColliderComponent &collider, std::set<world::Entity> &collisionCache)
		{
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

		void PhysicsManager::possibleCollissions(const ColliderComponent &collider, std::set<world::Entity> &collisionCache, std::vector<ColliderComponent> *out_colliders)
		{
			// FIXME store colliders in a QuadTree instead, so that we can partition
			for (auto it : m_colliders)
			{
				if (collider.getEntity() != it.second.getEntity() &&
					collisionCache.insert(it.second.getEntity()).second)
				{
					out_colliders->push_back(collider);
				}
			}
		}

	}
}
