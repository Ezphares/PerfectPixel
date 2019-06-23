#pragma once

#include <EntityComponentSystem/Processor.h>


namespace perfectpixel { namespace physics {

	class IntegratorProcessor : public ecs::Processor
	{
	public:
		IntegratorProcessor(ecs::EntityManager *entityManager);

		virtual void onProcess(const std::vector<ecs::Entity> &entities, types::PpFloat deltaT);
	};

}}