#pragma once

#include <EntityComponentSystem/IComponentStorage.h>
#include <EntityComponentSystem/System.h>

#include <Bedrock/numbers.h>
#include <Bedrock/PriorityVector.h>

#include <vector>
#include <map>

namespace perfectpixel {
	namespace ecs {

	class SystemQueue
	{
	public:
		static const int16_t USER_PROCESSOR_DEFAULT_PRIORITY;

	public:
		SystemQueue();
		virtual ~SystemQueue();

		void processAll(float deltaT);
		void renderAll(float deltaT);

		void registerSystem(System *system, int16_t priority, bool managed);

	private:
        bedrock::PriorityVector<System *> m_systems;
		std::vector<System *> m_managedProcessors;
	};

} }
