#pragma once

#include <EntityComponentSystem/System.h>

#include <Bedrock/numbers.h>

#include <vector>
#include <map>

namespace perfectpixel {
	namespace ecs {

	class ProcessorQueue
	{
	public:
		static const int32_t USER_PROCESSOR_DEFAULT_PRIORITY;

	public:
		ProcessorQueue();
		virtual ~ProcessorQueue();

		void processAll(float deltaT);
		void renderAll(float deltaT);

		void registerProcessor(System *processor, int32_t priority, bool managed);

	private:
		std::map<int32_t, std::vector<System*>> m_processors;
		std::vector<int32_t> m_priorities;
		std::vector<System *> m_managedProcessors;
	};

} }
