#pragma once

#include <EntityComponentSystem/Processor.h>
#include <EntityComponentSystem/LifecycleComponents.h>

#include <types/Logger.h>

#include <iostream>
#include <sstream>

namespace perfectpixel {
	namespace ecs {

	class DebugComponent : public HintComponent<DebugComponent> {};

	typedef QueryHelper<With<DebugComponent>> DebugQuery;

	class DebugProcessor : public Processor
	{
	public:
		DebugProcessor()
			: Processor(DebugQuery::build())
		{
		}

		virtual void onCreate(const std::vector<Entity> &entities)
		{
			for (Entity entity : entities)
			{
				PP_LOG(LEVEL_INFO, "Creating entity [" << entity << "]");
			}
		}

		virtual void onUpdate(const std::vector<Entity> &entities, types::PpFloat deltaT)
		{
			for (Entity entity : entities)
			{
				if (!CreationDoneLifecycleComponent::Has(entity))
				{
					PP_LOG(LEVEL_ERROR, "Processing entity [" << entity << "] before running creation");
				}
			}
		}

		virtual void onDestroy(const std::vector<Entity> &entities)
		{
			for (Entity entity : entities)
			{
				DebugComponent::Delete(entity);
				PP_LOG(LEVEL_INFO, "Destroying entity [" << entity << "]");
			}
		}
	};

} }


