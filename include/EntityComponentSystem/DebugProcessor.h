#pragma once

#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/ComponentRegistry.h>
#include <EntityComponentSystem/HintComponentContainer.h>
#include <EntityComponentSystem/LifecycleComponents.h>

#include <types/Logger.h>

#include <iostream>
#include <sstream>

namespace perfectpixel {
	namespace ecs {

		Component(DebugComponent) {};
		FinalizeComponent(DebugComponent);

		typedef QueryHelper<With<DebugComponent>> DebugQuery;

		class DebugProcessor : public Processor
		{
		public:
			DebugProcessor()
				: Processor(DebugQuery::build())
				, m_storage(new HintComponentContainer<DebugComponent>())
			{


				ComponentRegistry::Instance()->registerStorage(
					DebugComponent::getTypeId(),
					m_storage,
					true);
			}

			virtual void onCreate(const std::vector<Entity> &entities)
			{
				for (Entity entity : entities)
				{
					PP_LOG(LEVEL_INFO, "Creating entity [" << entity << "]");
				}
			}

			virtual void onProcess(const std::vector<Entity> &entities, types::PpFloat deltaT)
			{
				for (Entity entity : entities)
				{
					if (!ComponentRegistry::Instance()->getStorage<CreationDoneLifecycleComponent>()->hasComponent(entity))
					{
						PP_LOG(LEVEL_ERROR, "Processing entity [" << entity << "] before running creation");
					}
				}
			}

			virtual void onDestroy(const std::vector<Entity> &entities)
			{
				for (Entity entity : entities)
				{
					m_storage->removeComponent(entity);
					PP_LOG(LEVEL_INFO, "Destroying entity [" << entity << "]");
				}
			}

			HintComponentContainer<DebugComponent> *m_storage;

		};

} }


