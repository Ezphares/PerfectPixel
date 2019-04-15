#include <physics/PhysicsComponent.h>

#include <limits>

namespace perfectpixel
{
	namespace physics
	{

		PhysicsComponent::PhysicsComponent(
			ecs::Entity entity,
			types::PpFloat mass, 
			types::PpFloat bounciness,
			SimulationType simulation)
			: m_entity(entity)
			, m_mass(mass)
			, m_bounciness(bounciness)
			, m_simulation(simulation)
		{
		}

		PhysicsComponent::~PhysicsComponent()
		{
		}

		PhysicsComponent PhysicsComponent::staticCollider(ecs::Entity entity)
		{
			return PhysicsComponent(entity, std::numeric_limits<types::PpFloat>::infinity(), 0, KINEMATIC);
		}

		ecs::Entity PhysicsComponent::getEntity() const
		{
			return m_entity;
		}

		types::PpFloat PhysicsComponent::getMass()
		{
			return m_mass;
		}

		perfectpixel::types::PpFloat PhysicsComponent::getBounciness() const
		{
			return m_bounciness;
		}

		void PhysicsComponent::setMass(types::PpFloat mass)
		{
			m_mass = mass;
		}

	}
}