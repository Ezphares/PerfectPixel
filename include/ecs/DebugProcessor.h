#pragma once

#include "ecs/QuerySystem.h"
#include "ecs/LifecycleComponents.h"

#include "bedrock/Logger.h"

#include <iostream>
#include <sstream>

namespace perfectpixel {
	namespace ecs {

	class DebugComponent : public HintComponent<DebugComponent> {};

	typedef QueryHelper<With<DebugComponent>> DebugQuery;

	class DebugProcessor : public QuerySystem
	{
	public:
		DebugProcessor()
			: QuerySystem(DebugQuery::build())
		{
		}

		virtual void onCreate(const std::vector<Entity> &entities)
		{
			for (Entity entity : entities)
			{
				PP_LOG(LEVEL_INFO, "Creating entity [" << entity << "]");
			}
		}

		virtual void onUpdate(const std::vector<Entity> &entities, float deltaT)
		{
			(void)deltaT;
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


