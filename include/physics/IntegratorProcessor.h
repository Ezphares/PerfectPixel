#pragma once

#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/QueryHelper.h>
#include <EntityComponentSystem/TransformComponent.h>

namespace perfectpixel { namespace physics {

	typedef ecs::QueryHelper<ecs::With<ecs::TransformComponent>> IntegratorQuery;

	class IntegratorProcessor : public ecs::Processor
	{
	public:
		IntegratorProcessor(ecs::EntityManager *entityManager)
			: ecs::Processor(IntegratorQuery::build(entityManager))
		{
		}

		virtual void onProcess(const std::vector<ecs::Entity> &entities, types::PpFloat deltaT)
		{
			for (ecs::Entity entity : entities)
			{
				ecs::TransformComponent::Position(entity) += ecs::TransformComponent::Velocity(entity) * deltaT;
			}
		}
	};

}}